/**
 * \brief Implementation for emitter-callback to correctly place the particle emitter.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-12
 */

#include "EmitterUpdateCallback .h"

using namespace pbs17;

EmitterUpdateCallback::EmitterUpdateCallback(osgParticle::PointPlacer* p, osg::MatrixTransform* mt)
	: _placer(p), _trans(mt) {}


void EmitterUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
	//_placer->setCenter(_trans->getMatrix().getTrans());
	traverse(node, nv);
}
