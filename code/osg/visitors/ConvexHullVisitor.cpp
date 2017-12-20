/**
 * \brief Functionality for managing loaded models to prevent loading multiple times the same model.
 * The code is copied from http://www.vis-sim.com/osg/code/osgcode_bbox1.htm and adapted to our use.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-23
 */

#include "ConvexHullVisitor.h"

#include <iostream>

#include <osg/Geometry>

#include "../../graphics/ConvexHull3D.h"

using namespace pbs17;


/**
 * \brief Constructor. Initialize the visitor to traverse all children.
 *
 * \param globalTransform
 *      Global-transformation matrix (local to world).
 */
ConvexHullVisitor::ConvexHullVisitor(osg::Matrix globalTransform)
	: NodeVisitor(TRAVERSE_ALL_CHILDREN), _globalTransform(globalTransform), _vertices(new osg::Vec3Array), _convexHull(nullptr), _isCalculated(false) {
}


/**
 * \brief Calculate the bounding-box for the type osg::Geode.
 *
 * \param geode
 *      Current geode-child.
 */
void ConvexHullVisitor::apply(osg::Geode& geode) {

	// update bounding box for each drawable
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i) {
		osg::Geometry *curGeom = geode.getDrawable(i)->asGeometry();

		// Only process if the drawable is geometry
		if (curGeom) {
			osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(curGeom->getVertexArray());

			if (vertices) {
				// Store all vertices of the subtree
				for (unsigned int j = 0; j < vertices->size(); ++j) {
					osg::Vec3 vertex_at_j = (*vertices)[j] * _globalTransform;
					_vertices->push_back(vertex_at_j);
				}
			}
		} else {
			std::cout << "Consider using OBJ-files instead of shapes.";
		}
	}

	// continue traversing through the graph
	traverse(geode);
}


/**
 * \brief Calculate the bounding-box for the type osg::Billboard.
 * important to handle billboard so that its size will not affect the geode size continue traversing the graph
 *
 * \param node
 *      Current billboard-child.
 */
void ConvexHullVisitor::apply(osg::Billboard& node) {
	traverse(node);
}


/**
 * \brief Calculate the convex-hull based on all collected vertices.
 * 
 * \return Convex-hull object of the object (with rendering-geometry and vertice/face-arrays)
 */
ConvexHull3D* ConvexHullVisitor::getConvexHull() {
	if (!_isCalculated) {
		_convexHull = new ConvexHull3D(_vertices);
		_isCalculated = true;
	}

	return _convexHull;
}
