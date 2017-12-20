/**
 * \brief Functionality for managing loaded models to prevent loading multiple times the same model.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "ModelManager.h"

#include "Loader.h"

using namespace pbs17;


//! Pointer to the only instance of this class.
ModelManager* ModelManager::_pInstance = nullptr;


/**
 * \brief Singleton instance of the ModelManager-class.
 */
ModelManager* ModelManager::Instance() {
	// singleton-implementation => if there is not yet an instance initialized, create one.
	if (!_pInstance) {
		_pInstance = new ModelManager();
	}

	return _pInstance;
}


/**
 * \brief Load a Node-object from a model-file. It's implemented in a way that the model is loaded only once.
 * 
 * \param filePath
 *	    Complete path to the model to load.
 * \param useLod
 *      True if simplified models should be used if the camera is far away.
 *
 *  \return Model-node to attach to osg-nodes.
 */
osg::ref_ptr<osg::LOD> ModelManager::loadModel(std::string filePath, bool useLod) {
	// try to find the model, if it's found => return it and otherwise load it new
	osg::ref_ptr<osg::LOD> retModel;
	std::map<std::string, osg::ref_ptr<osg::LOD> >::iterator found = _loaded.find(filePath);

	if (found == _loaded.end()) {
		// model wasn't found => load and store it in the manager and return it
		osg::ref_ptr<osg::Node> modelL3 = Loader::loadModel(filePath, 1.0);
		osg::ref_ptr<osg::Node> modelL2 = Loader::loadModel(filePath, 0.5);
		osg::ref_ptr<osg::Node> modelL1 = Loader::loadModel(filePath, 0.1);

		retModel = new osg::LOD;

		if (useLod) {
			retModel->addChild(modelL1.get(), 50.0f, FLT_MAX);
			retModel->addChild(modelL2.get(), 10.0f, 50.0f);
			retModel->addChild(modelL3.get(), 0.0f, 10.0f);
		} else {
			retModel->addChild(modelL3.get(), 0.0f, FLT_MAX);
		}

		_loaded.insert(std::pair<std::string, osg::ref_ptr<osg::LOD> >(filePath, retModel));
	} else {
		// take found.
		retModel = _loaded[filePath];
	}

	return retModel;
}
