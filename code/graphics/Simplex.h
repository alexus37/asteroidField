/**
* \brief Representation of the simplex during the GJK-algorithm and EPA.
*
* \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
* \Date:   2017-11-29
*/

#pragma once

#include <algorithm>
#include <vector>

#include <Eigen/Core>

#include "Face.h"
#include "Edge.h"
#include "SupportPoint.h"

namespace pbs17 {

	/**
	 * \brief Class which represents the convex-hull and manages the structures between the different frameworks.
	 */
	class Simplex {
	public:
		/**
		 * \brief Constructor.
		 * 
		 * \param vertices
		 *      List of vertices which forms the simplex at the beginning.
		 */
		Simplex(std::vector<SupportPoint*> vertices)
			: _vertices(vertices) {}


		/**
		 * \brief Get the size of the simplex, which defines the shape of it (line, triangle or tetrahedron).
		 * 
		 * \return Size of the simplex points.
		 */
		size_t count() const {
			return _vertices.size();
		}


		/**
		 * \brief Access vertices directly with the [] operator.
		 * 
		 * \param i
		 *      Position of the wanted vertex.
		 * 
		 * \return Vertex at position i.
		 */
		SupportPoint* operator[](int i) {
			return _vertices[i];
		}


		/**
		 * \brief Add a new vertex.
		 * 
		 * \param v
		 *      New vertex of the simplex.
		 */
		void add(SupportPoint* v) {
			_vertices.push_back(v);
		}


		/**
		 * \brief Remove an existing vertex.
		 * 
		 * \param v
		 *      Vertex to remove off the simplex.
		 */
		void remove(SupportPoint* v) {
			_vertices.erase(std::remove(_vertices.begin(), _vertices.end(), v));
			delete v;
		}


		/**
		 * \brief Triangulate the simplex. The triangulation is done so that all faces/normals
		 * point outwards.
		 */
		void triangulate();


		/**
		 * \brief Only in EPA: Find the closest face to the origin
		 * 
		 * \return The face with all needed information for EPA.
		 */
		Face findClosestFace();


		/**
		 * \brief Only in EPA: Extend the triangulated simplex with a new point. All triangles which are
		 * visible to the point will be removed and replaced by new ones including the new point.
		 * 
		 * \param v
		 *      New point to extend the triangulated simplex.
		 * 
		 * \return True if the new vertex has been inserted; false otherwise (when vertex was already in the simplex)      
		 */
		bool extend(SupportPoint* v);

	private:

		//! All vertices which are used to define the simplex.
		std::vector<SupportPoint*> _vertices;

		//! All triangles which are used to define the simplex. (counter-clockwise)
		std::vector<Eigen::Vector3i> _triangles;


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
		bool isCorrectOrder(int a, int b, int c, int opposite);


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
		bool isCorrectOrder(int a, int b, int c, Eigen::Vector3d opposite);


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
		static void addEdge(std::vector<Edge> &edges, int a, int b);


		/**
		 * \brief Print the matlab-code in the console for the current simplex to plot in 3D.
		 */
		void printMatlabPlot() const;


		/**
		 * \brief Print the matlab-code in the console for the current simplex to plot in 3D.
		 * 
		 * \param closest
		 *      Print the face in the matlab-plot with a special color to see which is the closest to the origin.
		 */
		void printMatlabPlot(Face &closest) const;
	};
}
