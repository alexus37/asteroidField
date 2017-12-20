/**
 * \brief Representation of the simplex during the GJK-algorithm and EPA.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#include "Simplex.h"

#include <Eigen/Core>
// ReSharper disable once CppUnusedIncludeDirective
#include <Eigen/Geometry>
#include <limits>

#include "Geometry.h"
#include <iostream>

using namespace pbs17;


/**
 * \brief Triangulate the simplex. The triangulation is done so that all faces/normals
 * point outwards.
 */
void Simplex::triangulate() {
	if (_vertices.size() < 2 || _vertices.size() > 4) {
		throw "Unknown size of simplex. Only 2 to 4 vertices are supported.";
	}

	if (_vertices.size() == 2) {
		std::cout << "Simplex of 2\n";

		// If only a line is available at the moment when GJK converged, add two points to form a tetrahedron.
		// Create a new vertex which is perpendicular to the plane given by the two vertices and the origin.
		// Add another vertex which is close to the origin. With this, the hope is that after few steps the valid
		// and correct tetrahedron is computed and contains the origin (or at least that the origin is on the surface).
		Eigen::Vector3d o = Eigen::Vector3d(0, 0, 0);
		Eigen::Vector3d a = _vertices[0]->getMinkowskiPoint();
		Eigen::Vector3d b = _vertices[1]->getMinkowskiPoint();
		Eigen::Vector3d ba = a - b;
		Eigen::Vector3d bo = o - b;

		Eigen::Vector3d toC = ba.cross(bo);
		_vertices.push_back(new SupportPoint(a + toC.normalized() * 0.1, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0)));
		_vertices.push_back(new SupportPoint(b + (1.0 + EPS) * bo, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0)));
	}

	if (_vertices.size() == 3) {
		std::cout << "Simplex of 3\n";
	
		// If only a triangle is available at the moment when GJK converged, add one point to form a tetrahedron.
		// Add the point so that the origin is included or at least that the origin is on the surface.
		Eigen::Vector3d o = Eigen::Vector3d(0, 0, 0);
		Eigen::Vector3d b = _vertices[1]->getMinkowskiPoint();
		Eigen::Vector3d bo = o - b;

		_vertices.push_back(new SupportPoint(b + (1.0 + EPS) * bo, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0)));
	}

	// Triangulate the tetrahedron so that the normals point all outwards.
	if (isCorrectOrder(0, 1, 2, 3)) {
		_triangles.push_back(Eigen::Vector3i(0, 1, 2));
		_triangles.push_back(Eigen::Vector3i(0, 3, 1));
		_triangles.push_back(Eigen::Vector3i(0, 2, 3));
		_triangles.push_back(Eigen::Vector3i(1, 3, 2));
	} else {
		_triangles.push_back(Eigen::Vector3i(0, 2, 1));
		_triangles.push_back(Eigen::Vector3i(0, 1, 3));
		_triangles.push_back(Eigen::Vector3i(0, 3, 2));
		_triangles.push_back(Eigen::Vector3i(1, 2, 3));
	}
}



/**
 * \brief Only in EPA: Find the closest face to the origin.
 *
 * \return The face with all needed information for EPA.
 */
Face Simplex::findClosestFace() {
	Face closest;
	closest.setDistance(std::numeric_limits<double>::max());

	// Check each face if it is the closest or not.
	for (unsigned int i = 0; i < _triangles.size(); ++i) {
		SupportPoint* aSP = _vertices[_triangles[i].x()];
		SupportPoint* bSP = _vertices[_triangles[i].y()];
		SupportPoint* cSP = _vertices[_triangles[i].z()];
		Eigen::Vector3d a = aSP->getMinkowskiPoint();
		Eigen::Vector3d b = bSP->getMinkowskiPoint();
		Eigen::Vector3d c = cSP->getMinkowskiPoint();
		Eigen::Vector3d normal = getNormalFromPoints(a, b, c);
		double d = -normal.dot(a);

		double distance = abs(d / normal.norm());

		if (distance < closest.getDistance()) {
			closest.setDistance(distance);
			closest.setNormal(normal.normalized());
			closest.setVertices({ aSP, bSP, cSP });
		}
	}

	return closest;
}


/**
 * \brief Only in EPA: Extend the triangulated simplex with a new point. All triangles which are
 * visible to the point will be removed and replaced by new ones including the new point.
 *
 * \param v
 *      New point to extend the triangulated simplex.
 * 
 * \return True if the new vertex has been inserted; false otherwise (when vertex was already in the simplex)
 */
bool Simplex::extend(SupportPoint* v) {
	for (std::vector<SupportPoint*>::iterator it = _vertices.begin(); it != _vertices.end(); ++it) {
		if ((*it)->getMinkowskiPoint() == v->getMinkowskiPoint()) {
			return false;
		}
	}

	std::vector<Eigen::Vector3i> newTriangles;
	std::vector<Edge> edges;
	int newVertexPosition = static_cast<int>(_vertices.size());
	_vertices.push_back(v);

	// Check each triangle if the new point is visible:
	//  - yes: remove the trianlge and keep edge-information which edges to keep.
	//  - no:  keep the triangle
	for (unsigned int i = 0; i < _triangles.size(); ++i) {
		int a = _triangles[i].x();
		int b = _triangles[i].y();
		int c = _triangles[i].z();
		Eigen::Vector3d normal = getNormalFromPoints(_vertices[a]->getMinkowskiPoint(), _vertices[b]->getMinkowskiPoint(), _vertices[c]->getMinkowskiPoint());

		// Point is visible by face
		if (isSameDirection(normal, v->getMinkowskiPoint() - _vertices[a]->getMinkowskiPoint())) {
			addEdge(edges, a, b);
			addEdge(edges, b, c);
			addEdge(edges, c, a);
		} else {
			newTriangles.push_back(_triangles[i]);
		}
	}

	// Create new triangles based on the remaining edges and the new vertex.
	for (unsigned int i = 0; i < edges.size(); ++i) {
		newTriangles.push_back(Eigen::Vector3i(newVertexPosition, edges[i].getA(), edges[i].getB()));
	}

	_triangles = newTriangles;
	return true;
}


/**
 * \brief Check if the polygon defined by vertices a, b and c (in this order) is pointing outwards.
 *
 * \param a
 *      Index of 1st vertex.
 * \param b
 *      Index of 2nd vertex.
 * \param c
 *      Index of 3rd vertex.
 * \param opposite
 *      Index of the opposite vertex (4th vertex) to check if the normal is pointing outwards.
 */
bool Simplex::isCorrectOrder(int a, int b, int c, int opposite) {
	return isCorrectOrder(a, b, c, _vertices[opposite]->getMinkowskiPoint());
}


/**
 * \brief Check if the polygon defined by vertices a, b and c (in this order) is pointing outwards.
 *
 * \param a
 *      Index of 1st vertex.
 * \param b
 *      Index of 2nd vertex.
 * \param c
 *      Index of 3rd vertex.
 * \param opposite
 *      Position of the opposite vertex (4th vertex) to check if the normal is pointing outwards.
 */
bool Simplex::isCorrectOrder(int a, int b, int c, Eigen::Vector3d opposite) {
	Eigen::Vector3d normal = getNormalFromPoints(_vertices[a]->getMinkowskiPoint(), _vertices[b]->getMinkowskiPoint(), _vertices[c]->getMinkowskiPoint());
	Eigen::Vector3d height = opposite - _vertices[a]->getMinkowskiPoint();

	return isOppositeDirection(normal, height);
}


/**
 * \brief Only EPA: while removing the triangles which are seen by the added point, this keeps track
 * of the deleted edges. As soon as one edge is added a second time but in the opposite direction,
 * we know the edge is connecting two triangles which are both removed, so we can simply remove this
 * edge as well and no longer keep track of it.
 *
 * \param edges
 *      List of all observed (potentially removed) edges.
 * \param a
 *      Index of the start-vertex.
 * \param b
 *      Index of the end-vertex.
 */
void Simplex::addEdge(std::vector<Edge>& edges, int a, int b) {
	for (unsigned int i = 0; i < edges.size(); ++i) {
		if (edges[i].getA() == b && edges[i].getB() == a) {
			edges.erase(edges.begin() + i);
			return;
		}
	}

	edges.push_back(Edge(a, b));
}


/**
 * \brief Print the matlab-code in the console for the current simplex to plot in 3D.
 */
void Simplex::printMatlabPlot() const
{
	std::string x_0 = "", x_1 = "", x_2 = "";
	std::string y_0 = "", y_1 = "", y_2 = "";
	std::string z_0 = "", z_1 = "", z_2 = "";

	for (unsigned int i = 0; i < _triangles.size(); ++i) {
		Eigen::Vector3d a = _vertices[_triangles[i].x()]->getMinkowskiPoint();
		Eigen::Vector3d b = _vertices[_triangles[i].y()]->getMinkowskiPoint();
		Eigen::Vector3d c = _vertices[_triangles[i].z()]->getMinkowskiPoint();

		x_0 += " " + std::to_string(a.x());
		x_1 += " " + std::to_string(b.x());
		x_2 += " " + std::to_string(c.x());
		y_0 += " " + std::to_string(a.y());
		y_1 += " " + std::to_string(b.y());
		y_2 += " " + std::to_string(c.y());
		z_0 += " " + std::to_string(a.z());
		z_1 += " " + std::to_string(b.z());
		z_2 += " " + std::to_string(c.z());
	}

	std::cout << "X = [" << x_0 << ";" << x_1 << ";" << x_2 << "];" << std::endl
		<< "Y = [" << y_0 << ";" << y_1 << ";" << y_2 << "];" << std::endl
		<< "Z = [" << z_0 << ";" << z_1 << ";" << z_2 << "];" << std::endl
		<< "C = [ 0 0 1];" << std::endl
		<< "figure" << std::endl << "patch(X, Y, Z, C);" << std::endl;
}


/**
 * \brief Print the matlab-code in the console for the current simplex to plot in 3D.
 *
 * \param closest
 *      Print the face in the matlab-plot with a special color to see which is the closest to the origin.
 */
void Simplex::printMatlabPlot(Face &closest) const {
	std::string x_0 = "", x_1 = "", x_2 = "";
	std::string y_0 = "", y_1 = "", y_2 = "";
	std::string z_0 = "", z_1 = "", z_2 = "";

	for (unsigned int i = 0; i < _triangles.size(); ++i) {
		Eigen::Vector3d a = _vertices[_triangles[i].x()]->getMinkowskiPoint();
		Eigen::Vector3d b = _vertices[_triangles[i].y()]->getMinkowskiPoint();
		Eigen::Vector3d c = _vertices[_triangles[i].z()]->getMinkowskiPoint();

		x_0 += " " + std::to_string(a.x());
		x_1 += " " + std::to_string(b.x());
		x_2 += " " + std::to_string(c.x());
		y_0 += " " + std::to_string(a.y());
		y_1 += " " + std::to_string(b.y());
		y_2 += " " + std::to_string(c.y());
		z_0 += " " + std::to_string(a.z());
		z_1 += " " + std::to_string(b.z());
		z_2 += " " + std::to_string(c.z());
	}

	std::cout << "X = [" << x_0 << ";" << x_1 << ";" << x_2 << "];" << std::endl
		<< "Y = [" << y_0 << ";" << y_1 << ";" << y_2 << "];" << std::endl
		<< "Z = [" << z_0 << ";" << z_1 << ";" << z_2 << "];" << std::endl
		<< "C = [ 0 0 1];" << std::endl
		<< "X_closest = [ " << closest[0]->getMinkowskiPoint().x() << "; " << closest[1]->getMinkowskiPoint().x() << "; " << closest[2]->getMinkowskiPoint().x() << "];" << std::endl
		<< "Y_closest = [ " << closest[0]->getMinkowskiPoint().y() << "; " << closest[1]->getMinkowskiPoint().y() << "; " << closest[2]->getMinkowskiPoint().y() << "];" << std::endl
		<< "Z_closest = [ " << closest[0]->getMinkowskiPoint().z() << "; " << closest[1]->getMinkowskiPoint().z() << "; " << closest[2]->getMinkowskiPoint().z() << "];" << std::endl
		<< "C_closest = [ 1 0 0 ];" << std::endl << std::endl
		<< "figure" << std::endl << "patch(X, Y, Z, C);" << std::endl << "patch(X_closest, Y_closest, Z_closest, C_closest);" << std::endl;
}
