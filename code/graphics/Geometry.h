/**
 * \brief Helper-functions for simple geometric calculations
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#pragma once

#include <Eigen/Core>

namespace pbs17 {

	//! Define a small number number for a small threshold.
	const double EPS = 0.00001;


	/**
	 * \brief Check if two vectors are lookin in the same direction (+/- 90 degrees)
	 * 
	 * \param v1
	 *      Vector 1 to compare with.
	 * \param v2
	 *      Vector 2 to compare with.
	 * 
	 * \return True the direction of both vectors is the same.
	 */
	inline bool isSameDirection(Eigen::Vector3d v1, Eigen::Vector3d v2) {
		return v1.dot(v2) > 0;
	}


	/**
	 * \brief Check if two vectors are lookin in the opposite direction (+/- 90 degrees)
	 *
	 * \param v1
	 *      Vector 1 to compare with.
	 * \param v2
	 *      Vector 2 to compare with.
	 * 
	 * \return True the direction of both vectors is the opposite.
	 */
	inline bool isOppositeDirection(Eigen::Vector3d v1, Eigen::Vector3d v2) {
		return v1.dot(v2) < 0;
	}


	/**
	 * \brief Get the normal of a plane given three points which lies on the plane.
	 * 
	 * \param a
	 *      1st point on the plane.
	 * \param b
	 *      2nd point on the plane.
	 * \param c
	 *      3th point on the plane.
	 *      
	 * \return Normal of the plane. This is not normalized so that it can be reused to generate the plane equation.
	 */
	inline Eigen::Vector3d getNormalFromPoints(Eigen::Vector3d a, Eigen::Vector3d b, Eigen::Vector3d c) {
		Eigen::Vector3d v1 = a - b;
		Eigen::Vector3d v2 = c - b;
		return v1.cross(v2);
	}


	/**
	 * \brief Get the barycentric point on the triangle to calculate the intersection point.
	 * 
	 * \param p
	 *      Point which lies on the triangle/face.
	 * \param a
	 *      1st point/vertex of the triangle/face.
	 * \param b
	 *      2nd point/vertex of the triangle/face.
	 * \param c
	 *      3th point/vertex of the triangle/face.
	 * 
	 * \return Coordinates in the world-system of the intersection.
	 */
	inline Eigen::Vector3d barycentric(const Eigen::Vector3d &p, const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c) {
		Eigen::Vector3d v0 = b - a, v1 = c - a, v2 = p - a;
		double d00 = v0.dot(v0);
		double d01 = v0.dot(v1);
		double d11 = v1.dot(v1);
		double d20 = v2.dot(v0);
		double d21 = v2.dot(v1);
		double denom = d00 * d11 - d01 * d01;

		double v = (d11 * d20 - d01 * d21) / denom;
		double w = (d00 * d21 - d01 * d20) / denom;
		double u = 1.0f - v - w;

		return Eigen::Vector3d(u, v, w);
	}

	inline Eigen::Vector3d cartesian(const Eigen::Vector3d &bary, const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c) {
		return bary.x() * a + bary.y() * b + bary.z() * c;
	}
}
