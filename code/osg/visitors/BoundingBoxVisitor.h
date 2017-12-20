/**
 * \brief Functionality for managing loaded models to prevent loading multiple times the same model.
 * The code is copied from http://www.vis-sim.com/osg/code/osgcode_bbox1.htm and adapted to our use.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/Billboard>

namespace pbs17 {

	/**
	 * \brief Calculate the bounding-box of anode
	 */
	class  CalculateBoundingBox : public osg::NodeVisitor {

	public:

		/**
		 * \brief Constructor. Initialize the visitor to traverse all children.
		 */
		CalculateBoundingBox(osg::Matrix globalTransform, osg::Matrix localTransform) : NodeVisitor(TRAVERSE_ALL_CHILDREN) {
			_globalTransform = globalTransform;
			_localTransform = localTransform;

			_localBoundingBox = osg::BoundingBox();
			_globalBoundingBox = osg::BoundingBox();
		}


		/**
		 * \brief Destructor.
		 */
		virtual ~CalculateBoundingBox() {}


		/**
		 * \brief Calculate the bounding-box for the type osg::Geode.
		 *
		 * \param geode
		 *      Current geode-child.
		 */
		void apply(osg::Geode &geode) override;


		/**
		 * \brief Calculate the bounding-box for the type osg::MatrixTransform.
		 *
		 * \param node
		 *      Current matrix-transform-child.
		 */
		void apply(osg::MatrixTransform &node) override;


		/**
		 * \brief Calculate the bounding-box for the type osg::Billboard.
		 * important to handle billboard so that its size will not affect the geode size continue traversing the graph
		 *
		 * \param node
		 *      Current billboard-child.
		 */
		void apply(osg::Billboard &node) override;


		/**
		 * \brief Return the calculated bounding-box.
		 *
		 * \return Calculated bounding-box.
		 */
		osg::BoundingBox &getLocalBoundBox() {
			return _localBoundingBox;
		}
		osg::BoundingBox &getGlobalBoundBox() {
			return _globalBoundingBox;
		}

	protected:

		//! The overall resultant bounding box.
		osg::BoundingBox _localBoundingBox;
		osg::BoundingBox _globalBoundingBox;

		//! The current transform matrix.
		osg::Matrix _localTransform;
		osg::Matrix _globalTransform;
	};
}