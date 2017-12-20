/**
 * \brief Starting point for the program.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include <osg/Node>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <boost/program_options.hpp>

#include <string>
#include <iomanip>
#include <json.hpp>

#include <iostream>
#include <stdint.h>

#include "scene/SceneManager.h"
#include "physics/SimulationManager.h"
#include "osg/SnapImageDrawCallback.h"
#include "config.h"


using namespace boost::program_options;
// for convenience
using json = nlohmann::json;


int main(int argc, const char *argv[]) {
    pbs17::SceneManager* sceneManager = new pbs17::SceneManager;
    osg::ref_ptr<osg::Node> scene = nullptr;

	std::cout << std::fixed << std::setprecision(6);
    variables_map vm;

	try {
		options_description desc{ "Options" };
		desc.add_options()
			("help,h", "Help screen")
			("spheres,s", value<int>()->default_value(10), "Spheres")
			("asteroids,a", value<int>()->default_value(0), "Asteroids")
			("emitter,e", value<std::string>()->default_value("sphere"), "Emitter")
            ("rand,r", value<bool>()->default_value(true), "Random")
            ("gameplay,g", value<bool>()->default_value(false), "Gameplay")
            ("saveFrames,f", value<bool>()->default_value(false), "Save frame sto image files")
			("sceneJson,j", value<std::string>(), "Json file containing the scene");


		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		if (vm.count("help")) {
			std::cout << desc << '\n';
			return 0;
		} else if (vm.count("sceneJson")) {
			const std::string jsonFilePath = vm["sceneJson"].as<std::string>();

			// check if file exists
			std::ifstream stream(jsonFilePath);
			if (!stream) {
				stream = std::ifstream(SCENES_PATH + "/" + jsonFilePath);

				if (!stream) {
					std::cout << "File " + jsonFilePath + " doesnt exists!" << std::endl;
					return 0;
				}
			}

			// read json files
			json j;
			stream >> j;

			// load scene with json file
			std::cout << "load scene with json: " << jsonFilePath << '\n';
			scene = sceneManager->loadScene(j);
		} else {
			// load scene with parameters
			std::cout << "load scene with parameters" << '\n';
			scene = sceneManager->loadScene(vm);
		}
	}
	catch (const error &ex) {
		std::cerr << ex.what() << '\n';
	}






    //osg::ref_ptr<osg::Node> scene = sceneManager->loadScene();
	osg::ref_ptr<osgViewer::Viewer> viewer = sceneManager->initViewer(scene);
	osg::StateSet* state = scene->getOrCreateStateSet();
	state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	state->setMode(GL_LIGHT0, osg::StateAttribute::ON);


	// Set-up the screenshot functionality
	osg::ref_ptr<pbs17::SnapImageDrawCallback> screenshotCallback = new pbs17::SnapImageDrawCallback();
	viewer->getCamera()->setPostDrawCallback(screenshotCallback.get());

	pbs17::SimulationManager* simulationManager = new pbs17::SimulationManager(sceneManager->getSpaceObjects());

	double startTime = 0.0;
    bool captureFrame = vm["saveFrames"].as<bool>();


	while (!viewer->done()) {

        viewer->frame();

		long frameNumber = viewer->getFrameStamp()->getFrameNumber();
		double currentTime = viewer->elapsedTime();
		double dt = currentTime - startTime;
		std::cout << "Frame: " << frameNumber << "\tdt: " << dt << "\tfps: " << 1.0 / dt << std::endl;

        if(captureFrame) {
            std::string screenCaptureFilename =  std::to_string(frameNumber) + "_frame.png";
			osg::ref_ptr<pbs17::SnapImageDrawCallback> snapImageDrawCallback = dynamic_cast<pbs17::SnapImageDrawCallback*>(viewer->getCamera()->getPostDrawCallback());
			
        	if (snapImageDrawCallback.get()) {
				std::cout << "make screenshot" << std::endl;
				snapImageDrawCallback->setFileName(screenCaptureFilename);
				snapImageDrawCallback->setSnapImageOnNextFrame(true);
			}
        }

		dt = pbs17::SimulationManager::getSimulationDt();
		simulationManager->simulate(dt);

		startTime = currentTime;

	}

	delete sceneManager;
	delete simulationManager;

	return 0;
}
