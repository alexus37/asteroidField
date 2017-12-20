/**
 * \brief Functionality for collecting all the vertices in a given node.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <osg/NodeVisitor>
#include <osg/Billboard>

namespace pbs17 {

	/**
	 * \brief Calculate the bounding-box of anode
	 */
	class  VertexListVisitor : public osg::NodeVisitor {

	public:

		/**
		 * \brief Constructor. Initialize the visitor to traverse all children.
		 */
		VertexListVisitor() : NodeVisitor(TRAVERSE_ALL_CHILDREN) {

		}


		/**
		 * \brief Destructor.
		 */
		virtual ~VertexListVisitor() {}


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
		const osg::Vec3Array* &getVertices() {
			return m_vertices;
		}

	protected:

		//! Target vertex list
		const osg::Vec3Array* m_vertices;
	};
}