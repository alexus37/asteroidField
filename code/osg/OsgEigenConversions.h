/**
 * \brief Conversions between osg- and eigen-datatypes.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#pragma once

#include <Eigen/Core>

namespace pbs17 {
	// ---------------------------------------------------------------------- //
	// Vector conversions                                                     //
	// ---------------------------------------------------------------------- //
	
	/// Convert 2D vector of floats to OSG
	inline osg::Vec2f toOsg(const Eigen::Vector2f& vector) {
		osg::Vec2f osgVector;
		Eigen::Map<Eigen::Vector2f>(osgVector.ptr()) = vector;
		return osgVector;
	}

	/// Convert from OSG to 2D vector of floats
	inline Eigen::Vector2f fromOsg(const osg::Vec2f& vector) {
		return Eigen::Vector2f(vector.ptr());
	}

	/// Convert 2D vector of doubles to OSG
	inline osg::Vec2d toOsg(const Eigen::Vector2d& vector) {
		osg::Vec2d osgVector;
		Eigen::Map<Eigen::Vector2d>(osgVector.ptr()) = vector;
		return osgVector;
	}

	/// Convert from OSG to 2D vector of doubles
	inline Eigen::Vector2d fromOsg(const osg::Vec2d& vector) {
		return Eigen::Vector2d(vector.ptr());
	}

	/// Convert 3D vector of floats to OSG
	inline osg::Vec3f toOsg(const Eigen::Vector3f& vector) {
		osg::Vec3f osgVector;
		Eigen::Map<Eigen::Vector3f>(osgVector.ptr()) = vector;
		return osgVector;
	}
	/// Convert from OSG to 3D vector of floats
	inline Eigen::Vector3f fromOsg(const osg::Vec3f& vector) {
		return Eigen::Vector3f(vector.ptr());
	}

	/// Convert 3D vector of doubles to OSG
	inline osg::Vec3d toOsg(Eigen::Vector3d vector) {
		osg::Vec3d osgVector;
		Eigen::Map<Eigen::Vector3d>(osgVector.ptr()) = vector;
		return osgVector;
	}
	/// Convert from OSG to 3D vector of doubles
	inline Eigen::Vector3d fromOsg(const osg::Vec3d& vector) {
		return Eigen::Vector3d(vector.ptr());
	}

	/// Convert 4D vector of floats to OSG
	inline osg::Vec4f toOsg(const Eigen::Vector4f& vector) {
		osg::Vec4f osgVector;
		Eigen::Map<Eigen::Vector4f>(osgVector.ptr()) = vector;
		return osgVector;
	}
	/// Convert from OSG to 4D vector of floats
	inline Eigen::Vector4f fromOsg(const osg::Vec4f& vector) {
		return Eigen::Vector4f(vector.ptr());
	}

	/// Convert 4D vector of doubles to OSG
	inline osg::Vec4d toOsg(const Eigen::Vector4d& vector) {
		osg::Vec4d osgVector;
		Eigen::Map<Eigen::Vector4d>(osgVector.ptr()) = vector;
		return osgVector;
	}

	/// Convert from OSG to 4D vector of doubles
	inline Eigen::Vector4d fromOsg(const osg::Vec4d& vector) {
		return Eigen::Vector4d(vector.ptr());
	}


	// ---------------------------------------------------------------------- //
	// Matrix conversions                                                     //
	// ---------------------------------------------------------------------- //

	/// Convert a fixed-size 2x2 matrix of floats to OSG.
	template <int MOpt>
	osg::Matrix2 toOsg(const Eigen::Matrix<float, 2, 2, MOpt>& matrix) {
		osg::Matrix2 osgMatrix;
		Eigen::Map<Eigen::Matrix<float, 2, 2, Eigen::ColMajor> >(osgMatrix.ptr()) = matrix;
		return osgMatrix;
	}

	/// Convert a fixed-size 2x2 matrix of doubles to OSG.
	template <int MOpt>
	osg::Matrix2d toOsg(const Eigen::Matrix<double, 2, 2, MOpt>& matrix) {
		osg::Matrix2d osgMatrix;
		Eigen::Map<Eigen::Matrix<double, 2, 2, Eigen::ColMajor> >(osgMatrix.ptr()) = matrix;
		return osgMatrix;
	}

	/// Convert from OSG to a 2x2 matrix of floats.
	inline Eigen::Matrix<float, 2, 2, Eigen::RowMajor> fromOsg(const osg::Matrix2& matrix) {
		return Eigen::Map<const Eigen::Matrix<float, 2, 2, Eigen::ColMajor> >(matrix.ptr());
	}
	/// Convert from OSG to a 2x2 matrix of doubles.
	inline Eigen::Matrix<double, 2, 2, Eigen::RowMajor> fromOsg(const osg::Matrix2d& matrix) {
		return Eigen::Map<const Eigen::Matrix<double, 2, 2, Eigen::ColMajor> >(matrix.ptr());
	}

	/// Convert a fixed-size 3x3 matrix of floats to OSG.
	template <int MOpt> inline
		osg::Matrix3 toOsg(const Eigen::Matrix<float, 3, 3, MOpt>& matrix) {
		osg::Matrix3 osgMatrix;
		Eigen::Map<Eigen::Matrix<float, 3, 3, Eigen::ColMajor> >(osgMatrix.ptr()) = matrix;
		return osgMatrix;
	}

	/// Convert a fixed-size 3x3 matrix of doubles to OSG.
	template <int MOpt> inline
		osg::Matrix3d toOsg(const Eigen::Matrix<double, 3, 3, MOpt>& matrix) {
		osg::Matrix3d osgMatrix;
		Eigen::Map<Eigen::Matrix<double, 3, 3, Eigen::ColMajor> >(osgMatrix.ptr()) = matrix;
		return osgMatrix;
	}

	/// Convert from OSG to a 3x3 matrix of floats.
	inline Eigen::Matrix<float, 3, 3, Eigen::RowMajor> fromOsg(const osg::Matrix3& matrix) {
		return Eigen::Map<const Eigen::Matrix<float, 3, 3, Eigen::ColMajor> >(matrix.ptr());
	}

	/// Convert from OSG to a 3x3 matrix of doubles.
	inline Eigen::Matrix<double, 3, 3, Eigen::RowMajor> fromOsg(const osg::Matrix3d& matrix) {
		return Eigen::Map<const Eigen::Matrix<double, 3, 3, Eigen::ColMajor> >(matrix.ptr());
	}

	/// Convert a fixed-size 4x4 matrix of floats to OSG.
	template <int MOpt> inline
		osg::Matrixf toOsg(const Eigen::Matrix<float, 4, 4, MOpt>& matrix) {
		osg::Matrixf osgMatrix;
		Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::ColMajor> >(osgMatrix.ptr()) = matrix;
		return osgMatrix;
	}

	/// Convert from OSG to a 4x4 matrix of floats.
	inline Eigen::Matrix<float, 4, 4, Eigen::RowMajor> fromOsg(const osg::Matrixf& matrix) {
		return Eigen::Map<const Eigen::Matrix<float, 4, 4, Eigen::ColMajor> >(matrix.ptr());
	}

	/// Convert a fixed-size 4x4 matrix of doubles to OSG.
	template <int MOpt> inline
		osg::Matrixd toOsg(const Eigen::Matrix<double, 4, 4, MOpt>& matrix) {
		osg::Matrixd osgMatrix;
		Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::ColMajor> >(osgMatrix.ptr()) = matrix;
		return osgMatrix;
	}

	/// Convert from OSG to a 4x4 matrix of doubles.
	inline Eigen::Matrix<double, 4, 4, Eigen::RowMajor> fromOsg(const osg::Matrixd& matrix) {
		return Eigen::Map<const Eigen::Matrix<double, 4, 4, Eigen::ColMajor> >(matrix.ptr());
	}
}
