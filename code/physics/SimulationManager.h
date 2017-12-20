/**
 * \brief Implementation of the simulation manager which is called each frame.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <vector>
#include <algorithm>


// forward declarations
namespace pbs17 {
	class NBodyManager;
	class CollisionManager;
	class SpaceObject;
}


namespace pbs17 {

	/**
	 * \brief The manager to handle the simulation (within all needed calculations) to simulate one frame.
	 */
	class SimulationManager {
	public:
		/**
		 * \brief Constructor of the simulation-manager.
		 *
		 * \param spaceObjects
		 *      All space-objects in the scene.
		 */
		SimulationManager(std::vector<SpaceObject*> spaceObjects);


		/**
		 * \brief Simulate the scene.
		 *
		 * \param dt
		 *      Time difference since between the last frames.
		 */
		void simulate(double dt);


		/**
		 * \brief Set paused-state of the simulation
		 * 
		 * \param isPaused
		 *      True if the simulation is paused, false if it is running.
		 */
		static void setIsPaused(const bool isPaused) {
			IS_PAUSED = isPaused;
		}


		/**
		* \brief Set paused-state of the simulation
		*
		* \return True if the simulation is paused, false if it is running.
		*/
		static bool getIsPaused() {
			return IS_PAUSED;
		}


		/**
		 * \brief Set the simulation step. (Clamps to between 0.0 and 1.0).
		 * 
		 * \param dt
		 *      New simulation step.
		 */
		static void setSimulationDt(const double dt) {
			SIMULATION_DT = std::max(dt, 0.0);
			SIMULATION_DT = std::min(SIMULATION_DT, 1.0);
		}


		/**
		 * \brief Increase the simulation step. (Clamps to between 0.0 and 1.0).
		 *
		 * \param dt
		 *      Increasing-change of the simulation-step.
		 */
		static void increaseSimulationDt(const double dt) {
			setSimulationDt(SIMULATION_DT + dt);
		}


		/**
		 * \brief Decrease the simulation step. (Clamps to between 0.0 and 1.0).
		 *
		 * \param dt
		 *      Decrease-change of the simulation-step.
		 */
		static void decreaseSimulationDt(const double dt) {
			setSimulationDt(SIMULATION_DT - dt);
		}


		/**
		 * \brief Get the simulation step.
		 *
		 * \return Simulation-step.
		 */
		static double getSimulationDt() {
			return SIMULATION_DT;
		}


	private:

		//! All space-objects in the scene
		std::vector<SpaceObject*> _spaceObjects;
		//! Collision-manager for this scene
		CollisionManager* _cManager;
		//! Nbody-manager for this scene
		NBodyManager* _nManager;

		//! True if the simulation is paused
		static bool IS_PAUSED;

		//! Simulation-step (time-difference)
		static double SIMULATION_DT;
	};
}
