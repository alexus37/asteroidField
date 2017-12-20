/**
 * \brief Functionality to show the trace of the space-objects. This code is implemented with the help
 * of "Openscenegraph - cookbook".
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-16
 */

#include "TrailerCallback.h"

#include <osg/MatrixTransform>
#include <osg/Geometry>

#include "../FollowingRibbon.h"

using namespace pbs17;


/**
 * \brief Calculate the bounding-box for the type osg::Geode.
 *
 * \param node
 *      Current geode-child.
 * \param nv
 *	    Visitor of the tree.
 */
void TrailerCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
	osg::MatrixTransform* trans = static_cast<osg::MatrixTransform*>(node);
	
	if (trans && _geometry.valid()) {
		unsigned int numPoints = _ribbon->getNumPoints();
		float halfWidth = _ribbon->getHalfWidth();

		osg::Matrix matrix = trans->getMatrix();
		osg::Vec3Array* vertices = static_cast<osg::Vec3Array*>(_geometry->getVertexArray());
		osg::Vec3Array* normals = static_cast<osg::Vec3Array*>(_geometry->getNormalArray());

		for (unsigned int i = 0; i < numPoints - 3; i += 2) {
			(*vertices)[i] = (*vertices)[i + 2];
			(*vertices)[i + 1] = (*vertices)[i + 3];
			(*normals)[i] = (*normals)[i + 2];
			(*normals)[i + 1] = (*normals)[i + 3];
		}

		(*vertices)[numPoints - 2] = osg::Vec3(0.0f, -halfWidth, 0.0f) * matrix;
		(*vertices)[numPoints - 1] = osg::Vec3(0.0f, halfWidth, 0.0f) * matrix;
		vertices->dirty();

		osg::Vec3 normal = osg::Vec3(0.0f, 0.0f, 1.0f) * matrix;
		normal.normalize();
		(*normals)[numPoints - 2] = normal;
		(*normals)[numPoints - 1] = normal;

		normals->dirty();
		_geometry->dirtyBound();
	}

	traverse(node, nv);
}
