/**
 * \brief Visitor to compute the tangents.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-06
 */

#include "ComputeTangentVisitor.h"

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgUtil/TangentSpaceGenerator>

using namespace pbs17;


void ComputeTangentVisitor::apply(osg::Node &node) {
	traverse(node);
}


void ComputeTangentVisitor::apply(osg::Geode &geode) {
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i) {
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));

		if (geom) {
			generateTangentArray(geom);
		}
	}

	traverse(geode);
}


void ComputeTangentVisitor::generateTangentArray(osg::Geometry* geom) {
	osg::ref_ptr<osgUtil::TangentSpaceGenerator> tsg = new osgUtil::TangentSpaceGenerator;
	tsg->generate(geom);
	geom->setVertexAttribArray(6, tsg->getTangentArray());
	geom->setVertexAttribBinding(6, osg::Geometry::BIND_PER_VERTEX);
	geom->setVertexAttribArray(7, tsg->getBinormalArray());
	geom->setVertexAttribBinding(7, osg::Geometry::BIND_PER_VERTEX);
}
