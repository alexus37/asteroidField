/**
 * \brief Functionality for managing loaded models to prevent loading multiple times the same model.
 * The code is copied from http://www.vis-sim.com/osg/code/osgcode_bbox1.htm and adapted to our use.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <osg/NodeVisitor>
#include <osg/Billboard>
#include <osg/Geometry>

// Forward declarations
namespace pbs17 {
	class ConvexHull3D;
}

namespace pbs17 {

	/**
	 * \brief Calculate the bounding-box of anode
	 */
	class  ConvexHullVisitor : public osg::NodeVisitor {

	public:

		/**
		 * \brief Constructor. Initialize the visitor to traverse all children.
		 * 
		 * \param globalTransform
		 *      Global-transformation matrix (local to world).
		 */
		ConvexHullVisitor(osg::Matrix globalTransform);


		/**
		 * \brief Destructor.
		 */
		virtual ~ConvexHullVisitor() {}


		/**
		 * \brief Calculate the bounding-box for the type osg::Geode.
		 *
		 * \param geode
		 *      Current geode-child.
		 */
		void apply(osg::Geode &geode) override;

		
		/**
		 * \brief Calculate the bounding-box for the type osg::Billboard.
		 * important to handle billboard so that its size will not affect the geode size continue traversing the graph
		 *
		 * \param node
		 *      Current billboard-child.
		 */
		void apply(osg::Billboard &node) override;
		

		/**
		 * \brief Calculate the convex-hull based on all collected vertices.
		 *
		 * \return Convex-hull object of the object (with rendering-geometry and vertice/face-arrays)
		 */
		ConvexHull3D* getConvexHull();


	protected:

		//! All the vertices found of the subtree
		osg::Vec3Array* _vertices;

		//! The current transform matrix.
		osg::Matrix _globalTransform;

		//! The convex hull.
		ConvexHull3D* _convexHull;

		bool _isCalculated;
	
	};
}
