/**
 * \brief Implementation for the particle system to simulate smoke.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-12
 */

#pragma once

#include "SmokeParticleSystem.h"

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/FluidFrictionOperator>

#include "../../config.h"
#include "EmitterUpdateCallback .h"

using namespace pbs17;

SmokeParticleSystem::SmokeParticleSystem(osg::Group* scene, osg::MatrixTransform* mt) {
	/* Build the particle system in here ... */
	osgParticle::ParticleSystem* dustParticleSystem = new osgParticle::ParticleSystem;
	osg::Geode *geode = new osg::Geode();
	geode->addDrawable(dustParticleSystem);
	this->addChild(geode);

	// Set particle system state
	dustParticleSystem->setDefaultAttributes(DATA_PATH + "/texture/dust.png", true, false);

	// Define the Particle template
	osgParticle::Particle smokeParticle;
	smokeParticle.setSizeRange(osgParticle::rangef(0.2f, 0.4f)); // meters
	smokeParticle.setLifeTime(15); // seconds
	smokeParticle.setMass(0.4f);
	smokeParticle.setColorRange(osgParticle::rangev4(
		osg::Vec4(1.0, 1.0, 1.0, 1.0),
		osg::Vec4(0.0, 0.0, 0.0, 0.3f)));
	dustParticleSystem->setDefaultParticleTemplate(smokeParticle);

	// Add a ParticleSystemUpdater to the ParticleSystem
	osgParticle::ParticleSystemUpdater *dustSystemUpdater = new osgParticle::ParticleSystemUpdater;
	dustSystemUpdater->addParticleSystem(dustParticleSystem);
	this->addChild(dustSystemUpdater);

	// Add a ModularEmitter
	osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	emitter->setParticleSystem(dustParticleSystem);
	scene->addChild(emitter);

	osgParticle::RandomRateCounter *dustRate =
		static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
	dustRate->setRateRange(100, 400);

	//osgParticle::PointPlacer* _placer = static_cast<osgParticle::PointPlacer *>(emitter->getPlacer());
	//this->setUpdateCallback(new EmitterUpdateCallback(placer, mt));

	osgParticle::RadialShooter* shooter =
		static_cast<osgParticle::RadialShooter *>(emitter->getShooter());
	shooter->setThetaRange(0, 3.15149 / 20.0f); // radians, relative to Z axis.
	shooter->setInitialSpeedRange(1.4, 1.6); // meters/second
	emitter->setShooter(shooter);
}
