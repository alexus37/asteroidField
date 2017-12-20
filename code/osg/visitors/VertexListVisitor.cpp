/**
 * \brief Functionality for collecting all the vertices in a given node.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "VertexListVisitor.h"

#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <iostream>

using namespace pbs17;


/**
* \brief Calculate the bounding-box for the type osg::Geode.
*
* \param geode
*      Current geode-child.
*/
void VertexListVisitor::apply(osg::Geode& geode) {

	// update bounding box for each drawable
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i) {
		osg::Geometry *curGeom = geode.getDrawable(i)->asGeometry();

		// Only process if the drawable is geometry
		if (curGeom) {
			m_vertices = dynamic_cast<osg::Vec3Array*>(curGeom->getVertexArray());

			
		} else {
			std::cout << "Consider using OBJ-files instead of shapes.";
		}
	}

	// continue traversing through the graph
	traverse(geode);
}


/**
* \brief Calculate the bounding-box for the type osg::MatrixTransform.
*
* \param node
*      Current matrix-transform-child.
*/
void VertexListVisitor::apply(osg::MatrixTransform& node) {

	// continue traversing through the graph
	traverse(node);
}


/**
* \brief Calculate the bounding-box for the type osg::Billboard.
* important to handle billboard so that its size will not affect the geode size continue traversing the graph
*
* \param node
*      Current billboard-child.
*/
void VertexListVisitor::apply(osg::Billboard& node) {
	traverse(node);
}
