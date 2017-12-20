/**
 * \brief Implementation of the simulation manager which is called each frame.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "SimulationManager.h"

#include "../scene/SpaceObject.h"
#include "CollisionManager.h"
#include "NBodyManager.h"

using namespace pbs17;

bool SimulationManager::IS_PAUSED = false;
double SimulationManager::SIMULATION_DT = 0.01;;

/**
 * \brief Constructor of the simulation-manager.
 * 
 * \param spaceObjects
 *      All space-objects in the scene.
 */
SimulationManager::SimulationManager(std::vector<SpaceObject*> spaceObjects)
	: _spaceObjects(spaceObjects) {
    _nManager = new NBodyManager();

	// Todo: So far it is faster to let the simulation run on parallel-for-loop
	//_nManager->initSpatialGrid(spaceObjects, Eigen::Vector3i(30, 30, 30));
    _cManager = new CollisionManager(spaceObjects);
}


/**
 * \brief Simulate the scene.
 *
 * \param dt
 *      Time difference since between the last frames.
 */
void SimulationManager::simulate(double dt) {
	if (IS_PAUSED) {
		return;
	}

    // simulate on step
    _nManager->simulateStep(dt, this->_spaceObjects);

    // check for collisions
    _cManager->handleCollisions(dt, this->_spaceObjects);

    // TBD: check for fraction
}
