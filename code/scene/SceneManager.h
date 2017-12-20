/**
 * \brief Implementation of the scene manager to load and manage the scenes.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <osg/Group>
#include <osgViewer/Viewer>
#include <Eigen/Core>
#include <boost/program_options.hpp>
#include <json.hpp>


using namespace boost::program_options;
using json = nlohmann::json;


// Forward declarations
namespace pbs17 {
	class SpaceObject;
	class SpaceShip;
	class KeyboardHandler;
}

namespace pbs17 {
	/**
	 * \brief Load the objects for the scene and keep track of them.
	 */
	class SceneManager {
	public:
		/**
		 * \brief Constructor of the scene-manager.
		 */
		SceneManager();


		/**
		 * \brief Destructor of the scene-manager.
		 */
		virtual ~SceneManager();


		/**
		 * \brief Load the scene
		 * IMPORTANT: Add the "model" to the _spaceObjects for simulation-calculations and
		 *            and the "model->getModel()" to the scene for rendering.
		 *
		 * IMPORTANT: In a later step, we can define some file or parameters for creating the scene!
		 *            Just let me know if you have some ideas :-)
		 *
		 * \return Root-node of OSG for rendering.
		 */
		osg::ref_ptr<osg::Node> loadScene();

		
		/**
		 * \brief Load the scene based on the input parameters.
		 * 
		 * \param vm
		 *      Input parameters which have been passed by starting the program.
		 */
        osg::ref_ptr<osg::Node> loadScene(variables_map vm);


		/**
		 * \brief Load the scene based on the specified json-file.
		 *
		 * \param j
		 *      Loaded json-file.
		 */
        osg::ref_ptr<osg::Node> loadScene(json j);


		/**
		 * \brief Get multiple random samples in the 2d-space.
		 *
		 * \param pointCount
		 *      Number of random 2d-samples.
		 * \param random
		 *      Use a random generator (true) or align the samples uniformly on the border.
		 */
        std::vector<Eigen::Vector2d> getSamples(int pointCount, bool random) const;


		/**
		 * \brief Get multiple random samples in the 3d-space.
		 * 
		 * \param pointCount
		 *      Number of random 3d-samples.
		 * \param random
		 *      Use a random generator (true) or align the samples uniformly on the border.
		 */
        std::vector<Eigen::Vector3d> getSamples3(int pointCount, bool random) const;


		/**
		 * \brief Init the viewer for the rendering window and set up the camera.
		 * IMPORTANT: Camera can be animated in a later step! Or it can follow also some objects (not yet tested)
		 *
		 * \param scene
		 *      Root-node of OSG which contains the objects.
		 *
		 * \return OSG-viewer which handles the rendering
		 */
		osg::ref_ptr<osgViewer::Viewer> initViewer(osg::ref_ptr<osg::Node> scene) const;




		/**
		 * \brief Get the loaded space-objects.
		 *
		 * \return All space-objects in the scene.
		 */
		std::vector<SpaceObject*> getSpaceObjects() const {
			return _spaceObjects;
		}

	private:

		//! Root-node of OSG which contains the whole scene (used for rendering)
		osg::ref_ptr<osg::Group> _scene;

		//! All space-objects in the scene (used for calcualtions)
		std::vector<SpaceObject*> _spaceObjects;

		//! True if it is a game and false if it is a simulation
		bool _isGame = false;

		//! Spaceship for the player.
		SpaceShip *_player = nullptr;

		//! Keyboard-handler
		KeyboardHandler* _keyboardHandler = nullptr;

        const double DEFAULT_MASS = 500000;


		/**
		 * \brief Add the skybox to the scene.
		 */
		void addSkybox() const;


		/**
		* \brief Emmits the number of specified planets and asteroids. The space-objects are aligned in a sphere.
		*
		* \param planets
		*      Number of planets on the scene.
		* \param asteroids
		*      Number of asteroids on the scene.
		*/
		void sphereEmitter(int planets, int asteroids, bool random);


		/**
		* \brief Emmits the number of specified planets and asteroids. The space-objects are aligned in a cube.
		*
		* \param planets
		*      Number of planets on the scene.
		* \param asteroids
		*      Number of asteroids on the scene.
		*/
		void cubeEmitter(int planets, int asteroids, bool random);
	};
}
