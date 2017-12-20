/**
 * \brief Functionality for managing loaded images to prevent loading multiple times the same image.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#pragma once

#include <osg/Image>
#include <osg/Texture2D>

#include <map>


namespace pbs17 {

	/**
	 * \brief ImageManager manages already loaded images.
	 * This class prevents to load the same picture several times. If a picture is requested which was already loaded, it will return the already loaded image. Otherwise it will load it into the cache.
	 */
	class ImageManager {
	public:

		//! Singleton instance of the CGameState-class.
		static ImageManager* Instance();


		/**
		 * \brief Load a Texture2D-object from an image-file. It's implemented in a way that the image is loaded only once.
		 * 
		 * \param filePath
		 *      Complete path to the image-file.
		 * 
		 * \return Texture object to attach to osg-nodes.
		 */
		osg::ref_ptr<osg::Texture2D> loadTexture(std::string filePath);
		

		/**
		 * \brief Load a image-object from an image-file. It's implemented in a way that the image is loaded only once.
		 *
		 * \param filePath
		 *      Complete path to the image-file.
		 *
		 * \return Image object to attach to osg-nodes.
		 */
		osg::ref_ptr<osg::Image> loadImage(std::string filePath);


	private:

		//! All textures which have been loaded already.
		std::map<std::string, osg::ref_ptr<osg::Texture2D>> _textures;

		//! All images which have been loaded already.
		std::map<std::string, osg::ref_ptr<osg::Image>> _images;


		//! Private constructor to be sure the class can't be created outside of this class.
		ImageManager() {}

		//! Private copy-constructor to prevent copying the class.
		ImageManager(ImageManager const&) {};

		//! Private assignment-operator to prevent copying/saving referencec to the class.
		ImageManager& operator=(ImageManager const&) = delete;

		//! Private member-var which contains the singleton of the class.
		static ImageManager* _pInstance;
	};
}
