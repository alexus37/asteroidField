/**
 * \brief Implementation of the Gilbert-Johnson-Keerthi distance algorithm (GJK) and the expanding polytope algorithm (EPA).
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#pragma once

#include <Eigen/Core>

#include "Simplex.h"
#include "SupportPoint.h"
#include "../physics/Collision.h"

namespace pbs17 {

	/**
	 * \brief Implementation of the Gilbert-Johnson-Keerthi distance algorithm.
	 * Helping sources:
	 * - http://programyourfaceoff.blogspot.ch/2012/01/gjk-algorithm.html
	 * - http://www.dyn4j.org/2010/05/epa-expanding-polytope-algorithm
	 */
	class GjkAlgorithm {
	public:
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
		static bool intersect(std::vector<Eigen::Vector3d> &convex1, std::vector<Eigen::Vector3d> &convex2, Collision &collision);


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
		static SupportPoint* support(std::vector<Eigen::Vector3d> &convex1, std::vector<Eigen::Vector3d> &convex2, Eigen::Vector3d direction);


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
		static Eigen::Vector3d getFurthestPoint(std::vector<Eigen::Vector3d> &convex, Eigen::Vector3d direction);


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
		static bool processSimplex(Simplex &simplex, Eigen::Vector3d &direction);


	private:

		//! Choose a maximim number of iterations to avoid an infinite loop during a non-convergent search.
		static const int MAX_ITERATIONS;

		//! Tolerance of the EPA algorithm to determine if the hull can be still extended or not.
		static const double EPA_TOLERANCE;


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
		static bool processLine(Simplex &simplex, Eigen::Vector3d& direction);


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
		static bool processTriangle(Simplex &simplex, Eigen::Vector3d &direction);


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
		static bool processTetrahedron(Simplex &simplex, Eigen::Vector3d &direction);


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
		static bool EPA(Simplex &simplex, std::vector<Eigen::Vector3d> &convex1, std::vector<Eigen::Vector3d> &convex2, Collision &collision);
	};
}
