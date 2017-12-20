/**
 * \brief Functionality for representing the sky-box.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#pragma once

#include <osg/Transform>


namespace pbs17 {

	/**
	 * \brief The SkyBox representate the implementation for a cubemap for the sky visualisation.
	 * This class generates a box and applies the 6 different textures in a way that the user has the feeling to watch on a common environment.
	 */
	class SkyBox : public osg::Transform {
	public:

		/**
		 *\brief Default constructor. When this is used, don't forget to set up the textures with CSkyBox::setEnvironmentMap.
		 */
		SkyBox();


		/**
		 * \brief Constructor to initialize the cubemap for the sky imediately.
		 * 
		 * \param unit
		 *      Chanel of the object to put the texture on.
		 * \param texturePath
		 *      Path where all the files (sky-xneg.png, sky-xpos.png, ...) are located to read them into the cubemap.
		 *      The images located in the texturePath needs to be in .png format and labeled like "sky-xneg.png", "sky-xpos.png", ...
		 */
		SkyBox(unsigned int unit, std::string texturePath);


		/**
		 * \brief Destructor of SkyBox.
		 */
		virtual ~SkyBox();


		/**
		 * \brief Initialize the skybox with the correct openGL commands.
		 */
		void init();


		/**
		 * \brief Initialize the skybox with the correct openGL commands and with the texture.
		 * 
		 * \param unit
		 *      Chanel of the object to put the texture on.
		 * \param texturePath
		 *      Path where all the files (sky-xneg.png, sky-xpos.png, ...) are located to read them into the cubemap.
		 *      The images located in the texturePath needs to be in .png format and labeled like "sky-xneg.png", "sky-xpos.png", ...
		 */
		void init(unsigned int unit, std::string texturePath);


		/**
		 * \brief Set up the cubemap for the sky.
		 *
		 * \param unit
		 *      Chanel of the object to put the texture on.
		 * \param texturePath
		 *      Path where all the files (sky-xneg.png, sky-xpos.png, ...) are located to read them into the cubemap.
		 *      The images located in the texturePath needs to be in .png format and labeled like "sky-xneg.png", "sky-xpos.png", ...
		 */
		void setEnvironmentMap(unsigned int unit, std::string texturePath);


		/**
		 * \brief Set up the cubemap for the sky with the given osg-images.
		 * 
		 * \param unit
		 *      Chanel of the object to put the texture on.
		 * \param posX
		 *      Image used for positive x-axis.
		 * \param negX
		 *      Image used for negative x-axis.
		 * \param posY
		 *      Image used for positive y-axis.
		 * \param negY
		 *      Image used for negative y-axis.
		 * \param posZ
		 *      Image used for positive z-axis.
		 * \param negZ
		 *      Image used for negative z-axis.
		 */
		void setEnvironmentMap(unsigned int unit, osg::Image* posX, osg::Image* negX, osg::Image* posY, osg::Image* negY, osg::Image* posZ, osg::Image* negZ);


		/**
		 * \brief Computer the current local matrix to the world matrix. This is used as a visitor-pattern and only used for the CULL_VISITOR.
		 * 
		 * \param matrix
		 *      Matrix to probably convert.
		 * \param nv
		 *      Current node-visitor.
		 * 
		 * \return True if the matrix has been changed.
		 */
		bool computeLocalToWorldMatrix(osg::Matrixd& matrix, osg::NodeVisitor* nv) const override;


		/**
		 * \brief Computer the current world matrix to the local matrix. This is used as a visitor-pattern and only used for the CULL_VISITOR.
		 * 
		 * \param matrix
		 *      Matrix to probably convert.
		 * \param nv
		 *      Current node-visitor.
		 * 
		 * \return True if the matrix has been changed.
		 */
		bool computeWorldToLocalMatrix(osg::Matrixd& matrix, osg::NodeVisitor* nv) const override;

	};
}
