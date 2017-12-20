/**
 * \brief Helper-class to store the information of the triangles in the EPA.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#pragma once

#include <Eigen/Core>
#include "SupportPoint.h"

namespace pbs17 {
	/**
	 * \brief Helper-class for geometric processng. Stores a triangle on a mesh with the needed
	 * information during the EPA.
	 */
	class Face {
	public:
		/**
		 * \brief Empty constructor.
		 */
		Face()
			: Face(std::vector<SupportPoint*>(), 0, Eigen::Vector3d(1, 0, 0)) {}


		/**
		 * \brief Constructor to initialize all values.
		 * 
		 * \param vertices
		 *      The three vertices of the face.
		 * \param distance
		 *	    Distance from the face to the zero-point.
		 * \param normal
		 *      Normal of the face. (poinring outside)
		 */
		Face(std::vector<SupportPoint*> vertices, double distance, Eigen::Vector3d normal)
			: _vertices(vertices), _distance(distance), _normal(normal) {}



		/**
		 * \brief Get a single vertex by the position.
		 * 
		 * \param pos
		 *      Position of the vertex in the tiangle-definition.
		 * 
		 * \return Vertex at given position in the triangle.
		 */
		SupportPoint* operator[](const int pos) const {
			return _vertices[pos];
		}


		/**
		 * \brief Set all vertices at once.
		 * 
		 * \param vertices
		 *      New vertices of the triangle.
		 */
		void setVertices(const std::vector<SupportPoint*> vertices) {
			_vertices = vertices;
		}


		/**
		 * \brief Get distance of the triangle to the origin.
		 * 
		 * \return Distance between the triangle and the origin.
		 */
		double getDistance() const {
			return _distance;
		}


		/**
		 * \brief Set distance of the triangle to the origin.
		 * 
		 * \param distance
		 *      Distance between the triangle and the origin.
		 */
		void setDistance(const double distance) {
			_distance = distance;
		}

		
		/**
		 * \brief Get the normal of the triangle.
		 * 
		 * \return Normal of the triangle (normalized).
		 */
		Eigen::Vector3d getNormal() const {
			return _normal;
		}

		
		/**
		 * \brief Set the normal of the triangle.
		 * 
		 * 
		 */
		void setNormal(const Eigen::Vector3d normal) {
			_normal = normal;
		}
		
	private:
		Eigen::Vector3i _indices;
		std::vector<SupportPoint*> _vertices;
		double _distance;
		Eigen::Vector3d _normal;
	};
}
