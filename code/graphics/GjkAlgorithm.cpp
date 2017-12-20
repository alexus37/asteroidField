/**
 * \brief Implementation of the Gilbert-Johnson-Keerthi distance algorithm (GJK) and the expanding polytope algorithm (EPA).
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#include "GjkAlgorithm.h"

#include <vector>

#include <Eigen/Core>
// ReSharper disable once CppUnusedIncludeDirective
#include <Eigen/Geometry>
#include <limits>

#include "Geometry.h"

using namespace pbs17;

//! Choose a maximim number of iterations to avoid an infinite loop during a non-convergent search.
const int GjkAlgorithm::MAX_ITERATIONS = 50;

//! Tolerance of the EPA algorithm to determine if the hull can be still extended or not.
const double GjkAlgorithm::EPA_TOLERANCE = 0.00001;


/**
 * \brief Identifies if two convex-hulls intersect.
 *
 * \param convex1
 *      Convex-hull of first object.
 * \param convex2
 *      Convex-hull of second object.
 * \param collision
 *	    Output-parameter:
 *			Input:		Collision-object with the link to the two space-objects.
 *			Output:		Collision-object with all information of the intersection. (if there is any)
 *
 * \return True if there is an intersection, false otherwise.
 */
bool GjkAlgorithm::intersect(std::vector<Eigen::Vector3d> &convex1, std::vector<Eigen::Vector3d> &convex2, Collision &collision) {
	// Get an initial point on the Minkowski-sum.
	SupportPoint* s = support(convex1, convex2, Eigen::Vector3d(1.0, 1.0, 1.0));
	std::vector<SupportPoint*> sVertices;
	sVertices.push_back(s);

	// Create our initial simplex with the single point and initialize the search toward the origin.
	Simplex simplex(sVertices);
	Eigen::Vector3d d = -s->getMinkowskiPoint();

	// Try to find out if the origin is contained in the minkowski-sum or not.
	for (int i = 0; i < MAX_ITERATIONS; i++) {
		// Get our next simplex point toward the origin.
		SupportPoint* a = support(convex1, convex2, d);

		// If we move toward the origin and didn't pass it then we never will and there's no intersection.
		if (isOppositeDirection(a->getMinkowskiPoint(), d)) {
			return false;
		}

		// Add new point to the simplex and process it.
		simplex.add(a);

		// Here we either find a collision or we find the closest feature of
		// the simplex to the origin, make that the new simplex and update the direction
		// to move toward the origin from that feature.
		if (processSimplex(simplex, d)) {
			break;
		}
	}

	// Two cases:
	// - We are sure that there is a collision and we stopped the loop
	// - We still couldn't find a simplex that contains the origin and so we "probably" have an intersection
	EPA(simplex, convex1, convex2, collision);
	return true;
}


/**
 * \brief Get the furthest point on the Minkowski-sum on direction.
 *
 * \param convex1
 *      Convex-hull of first object.
 * \param convex2
 *      Convex-hull of second object.
 * \param direction
 *      Direction in which we are searching the support-function.
 *
 * \return Furthest point on the Minkowski-sum with corresponding two vertices of convex hull.
 */
SupportPoint* GjkAlgorithm::support(std::vector<Eigen::Vector3d> &convex1, std::vector<Eigen::Vector3d> &convex2, Eigen::Vector3d direction) {
	Eigen::Vector3d conv1Point = getFurthestPoint(convex1, direction);
	Eigen::Vector3d conv2Point = getFurthestPoint(convex2, -direction);
	return new SupportPoint(conv1Point - conv2Point, conv1Point, conv2Point);
}


/**
 * \brief Get the furthest point of the convex-hull of an object on the given axis.
 *
 * \param convex
 *      Convex-hull of considered object.
 * \param direction
 *      Axis/direction to search to furthest point.
 *
 * \return Furthest point.
 */
Eigen::Vector3d GjkAlgorithm::getFurthestPoint(std::vector<Eigen::Vector3d> &convex, Eigen::Vector3d direction) {
	double max = -std::numeric_limits<double>::max();
	Eigen::Vector3d maxV(0, 0, 0);

	for (unsigned int i = 0; i < convex.size(); ++i) {
		double dot = convex[i].dot(direction);

		if (dot > max) {
			max = dot;
			maxV = convex[i];
		}
	}

	return maxV;
}


/**
 * \brief Process the simplex with the given direction. It will find the intersection (containing the origin)
 * or the closest point to the origin.
 *
 * \param simplex
 *      Simplex used to search the origin.
 * \param direction
 *      Direction to extend the simplex with.
 *
 * \return True if the simplex contains the origin; false otherwise.
 */
bool GjkAlgorithm::processSimplex(Simplex &simplex, Eigen::Vector3d &direction) {
	switch (simplex.count()) {
	case 2:
		return processLine(simplex, direction);
	case 3:
		return processTriangle(simplex, direction);
	default:
		return processTetrahedron(simplex, direction);
	}
}


/**
 * \brief Check on which Voronoi-region the origin is. Then adjust the simplex to cut off regions.
 *
 * \param simplex
 *      Simplex used to search the origin.
 * \param direction
 *      Direction to extend the simplex with.
 *
 * \return False, since a line does not "include" the origin.
 */
bool GjkAlgorithm::processLine(Simplex &simplex, Eigen::Vector3d &direction) {
	SupportPoint* aSP = simplex[1];
	SupportPoint* bSP = simplex[0];
	Eigen::Vector3d a = aSP->getMinkowskiPoint();
	Eigen::Vector3d b = bSP->getMinkowskiPoint();
	Eigen::Vector3d ab = b - a;
	Eigen::Vector3d aO = -a;

	if (isSameDirection(ab, aO)) {
		direction = (ab.cross(aO)).cross(ab);
	} else {
		simplex.remove(bSP);
		direction = aO;
	}

	return false;
}


/**
 * \brief Check on which Voronoi-region the origin is. Then adjust the simplex to cut off regions.
 *
 * \param simplex
 *      Simplex used to search the origin.
 * \param direction
 *      Direction to extend the simplex with.
 *
 * \return False, since a triangle does not "include" the origin.
 */
bool GjkAlgorithm::processTriangle(Simplex &simplex, Eigen::Vector3d &direction) {
	SupportPoint* aSP = simplex[2];
	SupportPoint* bSP = simplex[1];
	SupportPoint* cSP = simplex[0];
	Eigen::Vector3d a = aSP->getMinkowskiPoint();
	Eigen::Vector3d b = bSP->getMinkowskiPoint();
	Eigen::Vector3d c = cSP->getMinkowskiPoint();
	Eigen::Vector3d ab = b - a;
	Eigen::Vector3d ac = c - a;
	Eigen::Vector3d abc = ab.cross(ac);
	Eigen::Vector3d aO = -a;
	Eigen::Vector3d acNormal = abc.cross(ac);
	Eigen::Vector3d abNormal = ab.cross(abc);

	if (isSameDirection(acNormal, aO)) {
		if (isSameDirection(ac, aO)) {
			simplex.remove(bSP);
			direction = (ac.cross(aO)).cross(ac);
		} else {
			if (isSameDirection(ab, aO)) {
				simplex.remove(cSP);
				direction = (ab.cross(aO)).cross(ab);
			} else {
				simplex.remove(bSP);
				simplex.remove(cSP);
				direction = aO;
			}
		}
	} else {
		if (isSameDirection(abNormal, aO)) {
			if (isSameDirection(ab, aO)) {
				simplex.remove(cSP);
				direction = (ab.cross(aO)).cross(ab);
			} else {
				simplex.remove(bSP);
				simplex.remove(cSP);
				direction = aO;
			}
		} else {
			if (isSameDirection(abc, aO)) {
				direction = (abc.cross(aO)).cross(abc);
			} else {
				direction = (-abc.cross(aO)).cross(-abc);
			}
		}
	}

	return false;
}


/**
 * \brief Check on which Voronoi-region the origin is. Then adjust the simplex to cut off regions.
 *
 * \param simplex
 *      Simplex used to search the origin.
 * \param direction
 *      Direction to extend the simplex with.
 *
 * \return True if the tetrahedron contains the origin; false otherwise.
 */
bool GjkAlgorithm::processTetrahedron(Simplex &simplex, Eigen::Vector3d &direction) {
	SupportPoint* aSP = simplex[3];
	SupportPoint* bSP = simplex[2];
	SupportPoint* cSP = simplex[1];
	SupportPoint* dSP = simplex[0];
	Eigen::Vector3d a = aSP->getMinkowskiPoint();
	Eigen::Vector3d b = bSP->getMinkowskiPoint();
	Eigen::Vector3d c = cSP->getMinkowskiPoint();
	Eigen::Vector3d d = dSP->getMinkowskiPoint();
	Eigen::Vector3d ac = c - a;
	Eigen::Vector3d ad = d - a;
	Eigen::Vector3d ab = b - a;

	Eigen::Vector3d acd = ad.cross(ac);
	Eigen::Vector3d abd = ab.cross(ad);
	Eigen::Vector3d abc = ac.cross(ab);

	Eigen::Vector3d aO = -a;

	if (isSameDirection(abc, aO)) {
		if (isSameDirection(abc.cross(ac), aO)) {
			simplex.remove(bSP);
			simplex.remove(dSP);
			direction = (ac.cross(aO)).cross(ac);
		} else if (isSameDirection(ab.cross(abc), aO)) {
			simplex.remove(cSP);
			simplex.remove(dSP);
			direction = (ab.cross(aO)).cross(ab);
		} else {
			simplex.remove(dSP);
			direction = abc;
		}
	} else if (isSameDirection(acd, aO)) {
		if (isSameDirection(acd.cross(ad), aO)) {
			simplex.remove(bSP);
			simplex.remove(cSP);
			direction = (ad.cross(aO)).cross(ad);
		} else if (isSameDirection(ac.cross(acd), aO)) {
			simplex.remove(bSP);
			simplex.remove(dSP);
			direction = (ac.cross(aO)).cross(ac);
		} else {
			simplex.remove(bSP);
			direction = acd;
		}
	} else if (isSameDirection(abd, aO)) {
		if (isSameDirection(abd.cross(ab), aO)) {
			simplex.remove(cSP);
			simplex.remove(dSP);
			direction = (ab.cross(aO)).cross(ab);
		} else if (isSameDirection(ad.cross(abd), aO)) {
			simplex.remove(bSP);
			simplex.remove(cSP);
			direction = (ad.cross(aO)).cross(ad);
		} else {
			simplex.remove(cSP);
			direction = abd;
		}
	} else {
		return true;
	}

	return false;
}


/**
 * \brief Expanding Polytope Algorithm: Extends the simplex which contains the origin so that the nearest point on the
 * Minkowski-sum to the origin is found.
 *
 * \param simplex
 *      Simplex of the GJK-algorithm containing the origin.
 * \param convex1
 *      Convex-hull of first object.
 * \param convex2
 *      Convex-hull of second object.
 * \param collision
 *	    Output-parameter:
 *			Input:		Collision-object with the link to the two space-objects.
 *			Output:		Collision-object with all information of the intersection. (if there is any)
 *
 * \return Direction of the colision.
 */
bool GjkAlgorithm::EPA(Simplex &simplex, std::vector<Eigen::Vector3d>& convex1, std::vector<Eigen::Vector3d>& convex2, Collision &collision) {
	simplex.triangulate();
	
	while (true) {
		bool stop = false;

		// Get the closest face to the origin.
		Face face = simplex.findClosestFace();
		Eigen::Vector3d normal = face.getNormal();
		double distance = face.getDistance();

		// Get the new support-point in the direction of the faces normal to expand the simplex.
		SupportPoint* p = support(convex1, convex2, normal);

		// Check the distance to the new support point.
		double d = abs(p->getMinkowskiPoint().dot(normal));

		// - New point does not expand the simplex => converged
		if (d - distance < EPA_TOLERANCE) {
			stop = true;
		}

		// - New point has already been visited once => converged (since jumping between vertices does not expand anything)
		if (!simplex.extend(p)) {
			stop = true;
		}

		// To finish the EPA, store the needed intersection-information on the collision
		if (stop) {
			Eigen::Vector3d bary = barycentric(p->getMinkowskiPoint(), face[0]->getMinkowskiPoint(), face[1]->getMinkowskiPoint(), face[2]->getMinkowskiPoint());
			Eigen::Vector3d poc1 = cartesian(bary, face[0]->getConvexHull1Point(), face[1]->getConvexHull1Point(), face[2]->getConvexHull1Point());
			Eigen::Vector3d poc2 = cartesian(bary, face[0]->getConvexHull2Point(), face[1]->getConvexHull2Point(), face[2]->getConvexHull2Point());

			collision.setFirstPOC(poc1);
			collision.setSecondPOC(poc2);
			collision.setIntersectionVector(d * normal);
			collision.setUnitNormal((collision.getFirstObject()->getPosition() - collision.getSecondObject()->getPosition()).normalized());

			return true;
		}
	}
}
