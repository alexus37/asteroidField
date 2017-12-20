/**
 * \brief Functionality for representing the sky-box.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#include "FollowingRibbon.h"

#include <osg/Geometry>

using namespace pbs17;


/**
 *\brief Default constructor. When this is used, don't forget to set up the textures with CSkyBox::setEnvironmentMap.
 */
FollowingRibbon::FollowingRibbon() {}


/**
 * \brief Initialize the skybox with the correct openGL commands.
 */
osg::Geometry* FollowingRibbon::init(osg::Vec3 startPosition, osg::Vec3 color, unsigned int numPoints, float halfWidth) {
	_numPoints = numPoints;
	_halfWidth = halfWidth;

	_vertices =	new osg::Vec3Array(_numPoints);
	_normals = new osg::Vec3Array(_numPoints);
	_colors = new osg::Vec4Array(_numPoints);

	_origin = startPosition;
	_normal = osg::Vec3(0.0f, 0.0f, 1.0f);

	for (unsigned int i = 0; i<_numPoints - 1; i += 2) {
		(*_vertices)[i] = _origin; (*_vertices)[i + 1] = _origin;
		(*_normals)[i] = _normal; (*_normals)[i + 1] = _normal;
		float alpha = sinf(osg::PI * static_cast<float>(i) / static_cast<float>(_numPoints));
		(*_colors)[i] = osg::Vec4(color, alpha);
		(*_colors)[i + 1] = osg::Vec4(color, alpha);
	}

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setDataVariance(osg::Object::DYNAMIC);
	geom->setUseDisplayList(false);
	geom->setUseVertexBufferObjects(true);
	
	geom->setVertexArray(_vertices.get());
	geom->setNormalArray(_normals.get());
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->setColorArray(_colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP, 0, _numPoints));

	return geom.release();
}

