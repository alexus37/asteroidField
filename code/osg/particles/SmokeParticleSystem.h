/**
 * \brief Implementation for the particle system to simulate smoke.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-12
 */

#pragma once

#include <osg/Group>
#include <osgParticle/PointPlacer>

namespace pbs17 {
	class SmokeParticleSystem : public osg::Group {
	public:
		SmokeParticleSystem(osg::Group* scene, osg::MatrixTransform* mt);

	private:
		osgParticle::PointPlacer* _placer;
	};
}
