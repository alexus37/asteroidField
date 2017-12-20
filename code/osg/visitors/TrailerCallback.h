/**
 * \brief Functionality to show the trace of the space-objects. This code is implemented with the help
 * of "Openscenegraph - cookbook".
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-16
 */

#pragma once

#include <osg/Geometry>
#include <osg/NodeCallback>

// Forward declarations
namespace pbs17 {
	class FollowingRibbon;
}

namespace pbs17 {

	/**
	 * \brief Calculate the bounding-box of anode
	 */
	class  TrailerCallback : public osg::NodeCallback {

	public:

		/**
		 * \brief Constructor. Initialize the visitor to traverse all children.
		 */
		TrailerCallback(FollowingRibbon* ribbon, osg::Geometry* geometry)
			: _ribbon(ribbon), _geometry(geometry) {}


		/**
		 * \brief Destructor.
		 */
		virtual ~TrailerCallback() {}


		/**
		 * \brief Calculate the bounding-box for the type osg::Geode.
		 *
		 * \param node
		 *      Current node-child.
		 * \param nv
		 *	    Visitor of the tree.
		 */
		void operator()(osg::Node* node, osg::NodeVisitor* nv) override;


	protected:

		// Reference to the following ribbon
		FollowingRibbon* _ribbon;
		
		//! Target vertex list
		osg::observer_ptr<osg::Geometry> _geometry;

	};
}
