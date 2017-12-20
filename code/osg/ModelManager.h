/**
 * \brief Functionality for managing loaded models to prevent loading multiple times the same model.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <map>

#include <osg/Node>
#include <osg/LOD>


namespace pbs17 {

	/**
	 * \brief ModelManager manages already loaded models.
	 * This class prevents to load the same model several times. If a model is requested which was already loaded, it will return the already loaded model. Otherwise it will load it into the cache.
	 */
	class ModelManager {
	public:

		/**
		 * \brief Singleton instance of the ModelManager-class.
		 */
		static ModelManager* Instance();


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
		osg::ref_ptr<osg::LOD> loadModel(std::string filePath, bool useLod = true);


	private:

		//! All models which have been loaded already.
		std::map<std::string, osg::ref_ptr<osg::LOD> > _loaded;


		//! Private constructor to be sure the class can't be created outside of this class.
		ModelManager() {}

		//! Private copy-constructor to prevent copying the class.
		ModelManager(ModelManager const&) {}

		//! Private assignment-operator to prevent copying/saving referencec to the class.
		ModelManager& operator=(ModelManager const&) = delete;

		//! Private member-var which contains the singleton of the class.
		static ModelManager* _pInstance;
	};
}
