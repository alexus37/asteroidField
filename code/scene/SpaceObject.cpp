/**
 * \brief Implementation of the space object (base class).
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "SpaceObject.h"

#include <osg/PolygonMode>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "../osg/OsgEigenConversions.h"
#include "../osg/visitors/BoundingBoxVisitor.h"
#include "../osg/ImageManager.h"
#include "../osg/visitors/ComputeTangentVisitor.h"
#include "../osg/shaders/BumpmapShader.h"
#include "../config.h"
#include "../osg/FollowingRibbon.h"
#include "../osg/visitors/TrailerCallback.h"

using namespace pbs17;


//! ID's start from 0.
long SpaceObject::RunningId = 0;


/**
* \brief Constructor of SpaceObject.
*
* \param filename
*      Relative location to the object-file. (Relative from the data-directory in the source).
*/
SpaceObject::SpaceObject(std::string filename, int i)
    : SpaceObject(filename, "") {
    std::cout << "should not be called" << std::endl;
}

SpaceObject::SpaceObject(json j) {
	_textureName = j["texture"].is_string() ? j["texture"].get<std::string>() : "";
	_bumpmapName = j["bumpmap"].is_string() ? j["bumpmap"].get<std::string>() : "";
    
	_filename = j["obj"].is_string()? j["obj"].get<std::string>(): "";
    _id = RunningId;
    ++RunningId;

    _position = Eigen::Vector3d(0, 0, 0);
    _orientation = osg::Quat(0, osg::X_AXIS);
;

    // For visually debuggin => Make bounding-box visible
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    _aabbShape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f));
    _aabbShape->setColor(osg::Vec4(1.0, 0, 0, 1.0));
    geode->addDrawable(_aabbShape);

    _aabbRendering = new osg::MatrixTransform;
    _aabbRendering->setNodeMask(0x1);
    _aabbRendering->addChild(geode.get());
    osg::StateSet* ss = _aabbRendering->getOrCreateStateSet();
    ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    ss->setAttributeAndModes(new osg::PolygonMode(
        osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
}


/**
* \brief Constructor of SpaceObject.
*
* \param filename
*      Relative location to the object-file. (Relative from the data-directory in the source).
* \param textureName
*      Relative location to the texture-file. (Relative from the data-directory in the source).
*/
SpaceObject::SpaceObject(std::string filename, std::string textureName)
	: _filename(filename), _textureName(textureName) {
	_id = RunningId;
	++RunningId;

	_position = Eigen::Vector3d(0, 0, 0);
	_orientation = osg::Quat(0, osg::X_AXIS);


	// For visually debuggin => Make bounding-box visible
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	_aabbShape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f));
	_aabbShape->setColor(osg::Vec4(1.0, 0, 0, 1.0));
	geode->addDrawable(_aabbShape);

	_aabbRendering = new osg::MatrixTransform;
	_aabbRendering->setNodeMask(0x1);
	_aabbRendering->addChild(geode.get());
	osg::StateSet* ss = _aabbRendering->getOrCreateStateSet();
	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	ss->setAttributeAndModes(new osg::PolygonMode(
		osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
}


/**
 * \brief Destructor of SpaceObject.
 */
SpaceObject::~SpaceObject() {
	if (_modelRoot) {
		_modelRoot = nullptr;
	}
}


/**
 * \brief Initialize the space-object for physics.
 *
 * \param mass
 *      Mass: unit = kg
 * \param linearVelocity
 *      Linear velocity: unit = m/s
 * \param angularVelocity
 *      Angular velocity: unit = rad/s
 * \param force
 *      Global force: unit = vector with norm equals to N
 * \param torque
 *      Global torque: unit = vector with norm equals to N*m (newton metre)
 */
void SpaceObject::initPhysics(double mass, Eigen::Vector3d linearVelocity, Eigen::Vector3d angularVelocity, Eigen::Vector3d force, Eigen::Vector3d torque) {
	_mass = mass;
	_linearVelocity = linearVelocity;
	_angularVelocity = angularVelocity;
	_force = force;
	_torque = torque;
}


/**
 * \brief Set the position of the object.
 *
 * \param newPosition
 *	    New position of the object.
 */
void SpaceObject::setPosition(Eigen::Vector3d newPosition) {
	_position = newPosition;

	osg::Matrixd rotation;
	_orientation.get(rotation);

	_transformation->setMatrix(rotation * osg::Matrix::translate(toOsg(newPosition)));
	calculateAABB();
}


/**
 * \brief Update the position and orientation of the space-object.
 *
 * \param newPosition
 *      New position of the object.
 * \param newOrientation
 *      New orientation of the object.
 */
void SpaceObject::updatePositionOrientation(Eigen::Vector3d newPosition, osg::Quat newOrientation) {
	_position = newPosition;
    _orientation = newOrientation;

	osg::Matrixd rotation;
    newOrientation.get(rotation);
	osg::Matrixd translation = osg::Matrix::translate(toOsg(newPosition));

    _transformation->setMatrix(rotation * translation);

	updateAABB();
}


/**
 * \brief Calculate the AABB for the object for it's current state.
 * Here, the correct min-max of each vector is considered.
 */
void SpaceObject::calculateAABB() {
	osg::Matrix scaling = osg::Matrix::scale(_scaling, _scaling, _scaling);
	osg::Matrix translation = osg::Matrix::translate(toOsg(_position));
	osg::Matrix rotation;
	_orientation.get(rotation);

	CalculateBoundingBox bbox(scaling * rotation * translation, scaling);
	_modelFile->accept(bbox);
	_aabbLocal = bbox.getLocalBoundBox();
	_aabbGlobal = bbox.getGlobalBoundBox();
	_aabbLocalOrig = bbox.getLocalBoundBox();
	_aabbGlobalOrig = bbox.getGlobalBoundBox();

	_aabbRendering->setMatrix(osg::Matrix::scale(_aabbGlobal._max - _aabbGlobal._min) * osg::Matrix::translate(toOsg(_position)));
}


/**
 * \brief Update the AABB. Here, only the 8 corners of the original AABB are
 * rotated/translated correctly to speed up the calculateion. (to calculate the
 * bounding box for each time step correclty over all vertices is too time consuming).
 */
void SpaceObject::updateAABB() {
	osg::Matrix translation = osg::Matrix::translate(toOsg(_position));
	osg::Matrix rotation;
	_orientation.get(rotation);

	osg::Matrix localToWorld = rotation * translation;

	osg::BoundingBox newGlobal;

	for (unsigned int i = 0; i < 8; ++i)
		newGlobal.expandBy(_aabbLocal.corner(i) * localToWorld);

	_aabbGlobal = newGlobal;
	_aabbRendering->setMatrix(osg::Matrix::scale(_aabbGlobal._max - _aabbGlobal._min) * osg::Matrix::translate(toOsg(_position)));
}


/**
 * \brief Reset the collision state to 0. Usually before each frame.
 */
void SpaceObject::resetCollisionState() {
	if (_collisionState == 0) {
		_aabbShape->setColor(osg::Vec4(1, 1, 1, 1));
	}

	_collisionState = 0;
}


/**
 * \brief Set the collision state of the object.
 *
 * \param c
 *      New collision-state. Possible values:
 *          - 0 = no collision
 *          - 1 = collision possible
 *          - 2 = collision for sure
 */
void SpaceObject::setCollisionState(int c) {
	_collisionState = std::max(_collisionState, c);

	_aabbShape->setColor(c == 1 ? osg::Vec4(0, 1, 0, 1) : osg::Vec4(1, 0, 0, 1));
}


/**
* \brief Get the convex hull with the correct global-vertex positions.
*
* \return List of vertices of the convex-hull in the global-world-space.
*/
std::vector<Eigen::Vector3d> SpaceObject::getConvexHull() const {
	// Todo: use Eigen-transformations instead
	osg::Matrix scaling = osg::Matrix::scale(_scaling, _scaling, _scaling);
	osg::Matrix translation = osg::Matrix::translate(toOsg(_position));
	osg::Matrix rotation;
	_orientation.get(rotation);

	std::vector<Eigen::Vector3d> transformed;
	std::vector<Eigen::Vector3d> current = _convexHull->getVertices();

	for (unsigned int i = 0; i < current.size(); ++i) {
		transformed.push_back(fromOsg(toOsg(current[i]) * rotation * translation * scaling));
	}

	return transformed;
}


/**
 * \brief Initialize the texture-properties and shader.
 */
void SpaceObject::initTexturing() {
	bool useBumpmap = _bumpmapName != "";
	osg::ref_ptr<osg::StateSet> stateset = _convexRenderSwitch->getOrCreateStateSet();

	// Apply bumpmap-shaders
	ComputeTangentVisitor ctv;
	ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	_modelFile->accept(ctv);

	// Load the texture
	if (_textureName != "") {
		std::string texturePath = DATA_PATH + "/texture/" + _textureName;
		osg::ref_ptr<osg::Texture2D> colorTex = ImageManager::Instance()->loadTexture(texturePath);

		if (useBumpmap) {
			std::string bumpmapPath = DATA_PATH + "/texture/" + _bumpmapName;
			osg::ref_ptr<osg::Texture2D> normalTex = ImageManager::Instance()->loadTexture(bumpmapPath);

			BumpmapShader bumpmapShader(colorTex, normalTex);
			bumpmapShader.apply(_convexRenderSwitch);
		} else {
			stateset->setTextureAttributeAndModes(0, colorTex.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}
	}

	// Define material-properties
	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setDiffuse(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 1.0));
	material->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material->setAmbient(osg::Material::FRONT, osg::Vec4f(0.4f, 0.4f, 0.4f, 1.0f));
	material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material->setShininess(osg::Material::FRONT, 100);
	stateset->setAttribute(material);
}


/**
 * \brief Initialize the geometry for the ribbon which follows the object to see
 * the object's trace.
 *
 * \param color
 *      Color of the following ribbon.
 * \param numPoints
 *      Number of points which are used to generate the ribbon. Higher value => longer ribbon.
 * \param halfWidth
 *      Width of the ribbon.
 */
void SpaceObject::initFollowingRibbon(osg::Vec3 color, unsigned int numPoints, float halfWidth) {
	FollowingRibbon* ribbon = new FollowingRibbon();
	osg::Geometry* geometry = ribbon->init(toOsg(_position), color, numPoints, halfWidth);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geometry);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	_transformation->addUpdateCallback(new TrailerCallback(ribbon, geometry));
	_modelRoot->addChild(geode);
}
