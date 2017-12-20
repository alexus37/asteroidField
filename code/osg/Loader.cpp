/**
 * \brief Functionality for loading models.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "Loader.h"

#include <iostream>

#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/Texture2D>

#include <osgUtil/Simplifier>

using namespace pbs17;


/**
 * \brief Load the specified model from the correct folder and return it as an open scene graph node.
 *
 * \param filePath
 *      Name of the model-file. (Full path to the model!)
 * \param ratio
 *      Ratio of the simplifier. (Supported values: [0..1])
 * \param scaling
 *      Scaling-factor to scale the model. (Supported values: ]0..inf]
 *
 * \return Node which can be added to the scene graph.
 */
osg::ref_ptr<osg::Node> Loader::loadModel(std::string filePath, float ratio, float scaling) {
	//std::cout << "Starting to load the model: \"" << filePath << "\"..." << std::endl;
	
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(filePath);

	if (!model) {
		std::cout << "File not found! Aborting..." << std::endl;
		exit(0);
	}

	model->setName(filePath);

	// Only simplify the node if desired.
	if (ratio != 1.0) {
		model = simplifyNode(model, ratio);
	}

	//std::cout << "Loaded: OK!" << std::endl;

	return model;
}


/**
 * \brief Simplify a given node with a specified ratio. This can result in better performance.
 *
 * \param node
 *      Node to simplify.
 * \param ratio
 *      Ratio of the simplifier.
 *
 * \return New simplified node which can be added to the scene graph.
 */
osg::ref_ptr<osg::Node> Loader::simplifyNode(osg::ref_ptr<osg::Node> node, float ratio) {
	//std::cout << "Simplifing node \"" << node->getName() << "\" with the ratio " << ratio << std::endl;

	osgUtil::Simplifier simplifier;
	simplifier.setSampleRatio(ratio);
	node->accept(simplifier);

	return node;
}


/**
 * \brief Scale a given node with a specified factor.
 *
 * \param node
 *      Node to scale.
 * \param scaling
 *      Scaling-factor to scale the model.
 *
 * \return New scaled node which can be added to the scene graph.
 */
osg::ref_ptr<osg::MatrixTransform> Loader::scaleNode(osg::ref_ptr<osg::Node> node, float scaling) {
	//std::cout << "Scaling node \"" << node->getName() << "\" with the ratio " << scaling << std::endl;

	osg::ref_ptr<osg::MatrixTransform> scaled = new osg::MatrixTransform;
	scaled->setMatrix(osg::Matrix::scale(scaling, scaling, scaling));
	scaled->addChild(node.get());

	return scaled;
}


/**
 * \brief Create a Texture2D-object from an image-file.
 *
 * \param filename
 *      Path to the image to load into the texture.
 *
 * \return Texture object to attach to osg-nodes.
 */
osg::ref_ptr<osg::Texture2D> Loader::loadTexture(std::string filename) {
	// Set up the texture for the walls and don't optimise this texture by OSG.
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;

	// load the image
	osg::Image* image = osgDB::readImageFile(filename);

	if (!image) {
		std::cout << "Couldn't find image: \"" << filename << "\" is missing!" << std::endl;
		exit(0);
	}

	texture->setImage(image);
	texture->setDataVariance(osg::Object::STATIC);
	texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::WrapMode::REPEAT);
	texture->setWrap(osg::Texture2D::WRAP_R, osg::Texture2D::WrapMode::REPEAT);

	return texture;
}


/**
 * \brief Load an image.
 *
 * \param filename
 *      Path to the image.
 *
 * \return Image object to attach to osg-nodes.
 */
osg::ref_ptr<osg::Image> Loader::loadImage(std::string filename) {
	// load the image
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);

	if (!image) {
		std::cout << "Couldn't find image: \"" << filename << "\" is missing! Aborting..." << std::endl;
		exit(0);
	}

	return image;
}
