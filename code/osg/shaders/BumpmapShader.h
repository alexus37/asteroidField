﻿/**
 * \brief Functionality for bumpmap-shading.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-12
 */

#pragma once

#include "Shader.h"
#include <osg/Texture2D>

namespace pbs17 {
	/**
	 * \brief The BumpmapShader is used to simulate the bumpmap/normalmap.
	 */
	class BumpmapShader : public Shader {

	public:

		/**
		 * \brief Constructor. Initializes the shader-programms.
		 *
		 * \param texture
		 *      The image-texture to apply.
		 * \param normals
		 *      The normal-texture to apply.
		 */
		BumpmapShader(osg::ref_ptr<osg::Texture2D> texture, osg::ref_ptr<osg::Texture2D> normals);


		/**
		 * \brief Destructor.
		 */
		virtual ~BumpmapShader();


		/**
		 * \brief Apply the shader to the given node.
		 * 
		 * \param node
		 *      The node to which the shader should be applied.
		 */
		void apply(osg::Node* node) override;


	private:

		//! The image-texture to apply.
		osg::ref_ptr<osg::Texture2D> _texture;
		//! The normal-texture to apply.
		osg::ref_ptr<osg::Texture2D> _normals;

	};
}

