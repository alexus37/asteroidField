/**
 * \brief Functionality for loading models.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Texture2D>


namespace pbs17 {
	/**
	 * \brief OsgLoader helps to easily load models and textures into the OSG.
	 * The OsgLoader contains some functions to load easily different models into the OSG with specified manipulators.
	 */
	class Loader {
	public:
		
		/**
		 * \brief Load the specified model from the correct folder and return it as an open scene graph node.
		 * 
		 * \param filename
		 *      Name of the model-file. (Full path to the model!)
		 * \param ratio
		 *      Ratio of the simplifier. (Supported values: [0..1])
		 * \param scaling
		 *      Scaling-factor to scale the model. (Supported values: ]0..inf]
		 * 
		 * \return Node which can be added to the scene graph.
		 */
		static osg::ref_ptr<osg::Node> loadModel(std::string filename, float ratio = 1.0, float scaling = 1.0);


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
		static osg::ref_ptr<osg::Node> simplifyNode(osg::ref_ptr<osg::Node> node, float ratio);


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
		static osg::ref_ptr<osg::MatrixTransform> scaleNode(osg::ref_ptr<osg::Node> node, float scaling);


		/**
		 * \brief Create a Texture2D-object from an image-file.
		 * 
		 * \param filename
		 *      Path to the image to load into the texture.
		 * 
		 * \return Texture object to attach to osg-nodes.
		 */
		static osg::ref_ptr<osg::Texture2D> loadTexture(std::string filename);


		/**
		 * \brief Load an image.
		 *
		 * \param filename
		 *      Path to the image.
		 *
		 * \return Image object to attach to osg-nodes.
		 */
		static osg::ref_ptr<osg::Image> loadImage(std::string filename);
	};
}