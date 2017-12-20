/**
 * \brief Helper-class to store the information of the deleted/remaining edges in the EPA.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#pragma once

namespace pbs17 {
	/**
	 * \brief Helper-class for geometric processng. Stores an edge on a mesh with the indices of
	 * both endpoints.
	 */
	class Edge {
	public:
		/**
		 * \brief Constructor to initialize an edge with its endpoint-indices. The indices must be 
		 * set so that the edge starts and ends on the triangle in the counter-clockwise direction.
		 * 
		 * \param a
		 *      Start-index.
		 * \param b
		 *      End-index.
		 */
		Edge(int a, int b)
			: _a(a), _b(b) {}


		/**
		 * \brief Get the index in the vertex-list of the start-point.
		 * 
		 * \return Index of start-point.
		 */
		int getA() const {
			return _a;
		}


		/**
		 * \brief Get the index in the vertex-list of the end-point.
		 *
		 * \return Index of end-point.
		 */
		int getB() const {
			return _b;
		}


	private:

		//! Index in the vertex-list of the start-point
		int _a;
		//! Index in the vertex-list of the end-point
		int _b;
	};
}