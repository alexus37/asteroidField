/**
 * \brief Functionality for representing the sky-box.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#include "SkyBox.h"
#include <osg/Depth>
#include <osg/TextureCubeMap>
#include <osgUtil/CullVisitor>
#include <osgDB/ReadFile>

#include "ImageManager.h"

using namespace pbs17;


/**
 *\brief Default constructor. When this is used, don't forget to set up the textures with CSkyBox::setEnvironmentMap.
 */
SkyBox::SkyBox() {}


/**
 * \brief Constructor to initialize the cubemap for the sky imediately.
 *
 * \param unit
 *      Chanel of the object to put the texture on.
 * \param texturePath
 *      Path where all the files (sky-xneg.png, sky-xpos.png, ...) are located to read them into the cubemap.
 *      The images located in the texturePath needs to be in .png format and labeled like "sky-xneg.png", "sky-xpos.png", ...
 */
SkyBox::SkyBox(unsigned int unit, std::string texturePath) {
	init(unit, texturePath);
}


/**
 * \brief Destructor of SkyBox.
 */
SkyBox::~SkyBox() {}


/**
 * \brief Initialize the skybox with the correct openGL commands.
 */
void SkyBox::init() {
	// initialize the node with different important openGL-settings.
	setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	setCullingActive(false);

	osg::StateSet* ss = getOrCreateStateSet();
	ss->setAttributeAndModes(new osg::Depth(osg::Depth::LEQUAL, 1.0f, 1.0f), osg::StateAttribute::ON);
	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	ss->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	ss->setRenderBinDetails(5, "RenderBin");
}


/**
 * \brief Initialize the skybox with the correct openGL commands and with the texture.
 *
 * \param unit
 *      Chanel of the object to put the texture on.
 * \param texturePath
 *      Path where all the files (sky-xneg.png, sky-xpos.png, ...) are located to read them into the cubemap.
 *      The images located in the texturePath needs to be in .png format and labeled like "sky-xneg.png", "sky-xpos.png", ...
 */
void SkyBox::init(unsigned int unit, std::string texturePath) {
	// initialize openGL.
	init();

	// initialize the cubemap with the files in the specified folder.
	setEnvironmentMap(unit, texturePath);
}


/**
 * \brief Set up the cubemap for the sky.
 *
 * \param unit
 *      Chanel of the object to put the texture on.
 * \param texturePath
 *      Path where all the files (sky-xneg.png, sky-xpos.png, ...) are located to read them into the cubemap.
 *      The images located in the texturePath needs to be in .png format and labeled like "sky-xneg.png", "sky-xpos.png", ...
 */
void SkyBox::setEnvironmentMap(unsigned int unit, std::string texturePath) {
	// initialize the cubemap with the files in the specified folder
	setEnvironmentMap(0,
		ImageManager::Instance()->loadImage(texturePath + "/sky-xneg.png"),
		ImageManager::Instance()->loadImage(texturePath + "/sky-xpos.png"),
		ImageManager::Instance()->loadImage(texturePath + "/sky-yneg.png"),
		ImageManager::Instance()->loadImage(texturePath + "/sky-ypos.png"),
		ImageManager::Instance()->loadImage(texturePath + "/sky-zneg.png"),
		ImageManager::Instance()->loadImage(texturePath + "/sky-zpos.png")
	);
}


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
void SkyBox::setEnvironmentMap(unsigned int unit, osg::Image* posX, osg::Image* negX, osg::Image* posY, osg::Image* negY, osg::Image* posZ, osg::Image* negZ) {
	if (posX && posY && posZ && negX && negY && negZ) {
		// initialize the cubemap with the different pictures
		osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, posX);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, negX);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, posY);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, negY);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, posZ);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, negZ);

		// set up the wraps for the textures
		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
		cubemap->setResizeNonPowerOfTwoHint(false);

		getOrCreateStateSet()->setTextureAttributeAndModes(unit, cubemap.get());
	}
}


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
bool SkyBox::computeLocalToWorldMatrix(osg::Matrixd& matrix, osg::NodeVisitor* nv) const {
	if (nv && nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR) {
		osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(nv);
		matrix.preMult(osg::Matrix::translate(cv->getEyeLocal()));
		return true;
	} else {
		return osg::Transform::computeLocalToWorldMatrix(matrix, nv);
	}
}


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
bool SkyBox::computeWorldToLocalMatrix(osg::Matrixd& matrix, osg::NodeVisitor* nv) const {
	if (nv && nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR) {
		osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(nv);
		matrix.postMult(osg::Matrix::translate(-cv->getEyeLocal()));
		return true;
	} else {
		return osg::Transform::computeWorldToLocalMatrix(matrix, nv);
	}
}
