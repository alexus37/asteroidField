/**
* \brief Helper-class to store the information of the vertices during the GJK and EPA.
*
* \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
* \Date:   2017-12-03
*/

#pragma once

#include <Eigen/Core>

namespace pbs17 {
	/**
	 * \brief A vertex in the minkowski-sum consists as the difference of two vertices from each convex-hull.
	 * To keep track of all, this class can be used.
	 */
	class SupportPoint {
	public:

		/**
		 * \brief Empty constructor.
		 */
		SupportPoint()
			: SupportPoint(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0)) {}


		/**
		 * \brief Constructor to initialize the three needed points.
		 * 
		 * \param minkowskiPoint
		 *      Point on the minkowski-sum.
		 * \param _convexHull1Point
		 *      Support-vertex from the convex-hull 1.
		 * \param _convexHull2Point
		 *      Support-vertex from the convex-hull 2.
		 */
		SupportPoint(Eigen::Vector3d minkowskiPoint, Eigen::Vector3d _convexHull1Point, Eigen::Vector3d _convexHull2Point)
			: _minkowskiPoint(minkowskiPoint), _convexHull1Point(_convexHull1Point), _convexHull2Point(_convexHull2Point) {}


		/**
		 * \brief Get the point on the minkowski-sum.
		 * 
		 * \return Point on the minkowski-sum.
		 */
		Eigen::Vector3d getMinkowskiPoint() const {
			return _minkowskiPoint;
		}


		/**
		 * \brief Get the support-vertex from the convex-hull 1.
		 * 
		 * \return Support-vertex from the convex-hull 1.
		 */
		Eigen::Vector3d getConvexHull1Point() const {
			return _convexHull1Point;
		}


		/**
		 * \brief Get the support-vertex from the convex-hull 2.
		 *
		 * \return Support-vertex from the convex-hull 2.
		 */
		Eigen::Vector3d getConvexHull2Point() const {
			return _convexHull2Point;
		}


	private:

		//! Point on the minkowski-sum.
		Eigen::Vector3d _minkowskiPoint;
		//! Support-vertex from the convex-hull 1.
		Eigen::Vector3d _convexHull1Point;
		//! Support-vertex from the convex-hull 2.
		Eigen::Vector3d _convexHull2Point;
	};
}