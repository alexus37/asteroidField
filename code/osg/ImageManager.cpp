/**
 * \brief Functionality for managing loaded images to prevent loading multiple times the same image.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#include "ImageManager.h"

#include "Loader.h"

using namespace pbs17;


//! Pointer to the only instance of this class.
ImageManager* ImageManager::_pInstance = nullptr;


/**
* \brief Singleton instance of the ImageManager-class.
*/
ImageManager* ImageManager::Instance() {
	// singleton-implementation => if there is not yet an instance initialized, create one.
	if (!_pInstance) {
		_pInstance = new ImageManager();
	}

	return _pInstance;
}


/**
 * \brief Load a Texture2D-object from an image-file. It's implemented in a way that the image is loaded only once.
 *
 * \param filePath
 *      Complete path to the image-file.
 *
 * \return Texture object to attach to osg-nodes.
 */
osg::ref_ptr<osg::Texture2D> ImageManager::loadTexture(std::string filePath) {
	// try to find the texture, if it's found => return it and otherwise load it new
	osg::ref_ptr<osg::Texture2D> retTexture;
	std::map<std::string, osg::ref_ptr<osg::Texture2D>>::iterator found = _textures.find(filePath);

	if (found == _textures.end()) {
		// texture wasn't found => load and store it in the manager and return it
		retTexture = Loader::loadTexture(filePath);
		_textures.insert(std::pair<std::string, osg::ref_ptr<osg::Texture2D>>(filePath, retTexture));
	} else {
		// take found.
		retTexture = _textures[filePath];
	}

	return retTexture;
}


/**
 * \brief Load a image-object from an image-file. It's implemented in a way that the image is loaded only once.
 *
 * \param filePath
 *      Complete path to the image-file.
 *
 * \return Image object to attach to osg-nodes.
 */
osg::ref_ptr<osg::Image> ImageManager::loadImage(std::string filePath) {
	// try to find the texture, if it's found => return it and otherwise load it new
	osg::ref_ptr<osg::Image> retImage;
	std::map<std::string, osg::ref_ptr<osg::Image>>::iterator found = _images.find(filePath);

	if (found == _images.end()) {
		// texture wasn't found => load and store it in the manager and return it
		retImage = Loader::loadImage(filePath);
		_images.insert(std::pair<std::string, osg::ref_ptr<osg::Image>>(filePath, retImage));
	} else {
		// take found.
		retImage = _images[filePath];
	}

	return retImage;
}