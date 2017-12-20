/**
 * \brief Implementation of the asteroid.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "Asteroid.h"

#include <osgDB/ReadFile>
#include <osg/Switch>

#include "../config.h"
#include "../osg/JsonEigenConversions.h"
#include "../osg/OsgEigenConversions.h"
#include "../osg/Loader.h"
#include "../osg/ModelManager.h"
#include "../osg/visitors/ConvexHullVisitor.h"
#include "../osg/visitors/VertexListVisitor.h"

using namespace pbs17;


/**
* \brief Constructor of Asteroid.
*/
Asteroid::Asteroid()
    : SpaceObject("A2.obj", 0) {
}


/**
 * \brief Constructor of Asteroid with JSON-configuration.
 * 
 * \param j
 *      JSON-configuration for the asteroid.
 */
Asteroid::Asteroid(json j) :
    SpaceObject(j) {

    Eigen::Vector3d pos = fromJson(j["position"]);
    initOsg(pos, j["ratio"].get<double>(), j["scaling"].get<double>());

	Eigen::Vector3d linearVelocity = fromJson(j["linearVelocity"]);
    Eigen::Vector3d angularVelocity = fromJson(j["angularVelocity"]);
    Eigen::Vector3d force = fromJson(j["force"]);
    Eigen::Vector3d torque = fromJson(j["torque"]);

    initPhysics(j["mass"].get<double>(),linearVelocity,angularVelocity,force, torque);

	if (j["useFollowingRibbon"].is_boolean() && j["useFollowingRibbon"].get<bool>() == true) {
		json ribbonInfo = j["followingRibbon"];

		initFollowingRibbon(toOsg(fromJson(ribbonInfo["color"])),
			ribbonInfo["numPoints"].get<unsigned int>(),
			ribbonInfo["halfWidth"].get<float>());
	}
}


/**
 * \brief Destructor of Asteroid.
 */
Asteroid::~Asteroid() {}


/**
 * \brief Initialize the space-object for OSG.
 *
 * \param position
 *      Initial position of the object.
 * \param ratio
 *      Ratio of the simplifier. (Supported values: [0..1])
 * \param scaling
 *      Scaling of the model. (1.0 => not scaled, < 1.0 => smaller, > 1.0 => larger)
 */
void Asteroid::initOsg(Eigen::Vector3d position, double ratio, double scaling) {
	// Set the position to the space-object
	_position = position;
	_scaling = scaling;

	// Load the model
	std::string modelPath = DATA_PATH + "/" + _filename;
	_modelFile = ModelManager::Instance()->loadModel(modelPath, true);

	// Scale the model if needed
	if (scaling != 1.0) {
		_modelFile = Loader::scaleNode(_modelFile, scaling);
	}

	// Compute convex hull
	osg::Matrix scalingMatrix = osg::Matrix::scale(_scaling, _scaling, _scaling);
	ConvexHullVisitor convexHull(scalingMatrix);
	_modelFile->accept(convexHull);
	_convexHull = convexHull.getConvexHull();
	
	osg::Geode* geodeConvexHull = new osg::Geode;
	geodeConvexHull->addDrawable(_convexHull->getOsgModel());

	// Switch to decide if the convex hull or the model has to be rendered.
	_convexRenderSwitch = new osg::Switch;
	_convexRenderSwitch->addChild(_modelFile, true);
	_convexRenderSwitch->addChild(geodeConvexHull, false);

    VertexListVisitor vListVisitor;
    _modelFile->accept(vListVisitor);
	const osg::Vec3Array* vertices = dynamic_cast<const osg::Vec3Array*>(vListVisitor.getVertices());

	double minX = DBL_MAX, maxX = -DBL_MAX, minY = DBL_MAX, maxY = -DBL_MAX, minZ = DBL_MAX, maxZ = -DBL_MAX;

    for (unsigned int i = 0; i < vertices->size(); i++) {
		minX = std::min(minX, static_cast<double>(vertices->at(i).x()));
		minY = std::min(minY, static_cast<double>(vertices->at(i).y()));
		minZ = std::min(minZ, static_cast<double>(vertices->at(i).z()));

		maxX = std::max(maxX, static_cast<double>(vertices->at(i).x()));
		maxY = std::max(maxY, static_cast<double>(vertices->at(i).y()));
		maxZ = std::max(maxZ, static_cast<double>(vertices->at(i).z()));
	}

	double a = maxX - minX;
	double b = maxY - minY;
	double c = maxZ - minZ;
	double Ixx = 1. / 12.*(b*b + c*c) * scaling * scaling;
	double Iyy = 1. / 12.*(a*a + c*c) * scaling * scaling;
	double Izz = 1. / 12.*(a*a + b*b) * scaling * scaling;

	_momentOfInertia = Eigen::Matrix3d();
	_momentOfInertia.setIdentity();
	_momentOfInertia(0, 0) = Ixx;
	_momentOfInertia(1, 1) = Iyy;
	_momentOfInertia(2, 2) = Izz;

	// Transformation-node for position and rotation updates.
	_transformation = new osg::MatrixTransform;
	_transformation->setMatrix(osg::Matrix::translate(toOsg(position)));
	_transformation->addChild(_convexRenderSwitch);

	calculateAABB();

	_modelRoot = new osg::Switch;
	_modelRoot->insertChild(0, _transformation, true);
	_modelRoot->insertChild(1, _aabbRendering, false);

	initTexturing();
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
void Asteroid::initPhysics(double mass, Eigen::Vector3d linearVelocity, Eigen::Vector3d angularVelocity, Eigen::Vector3d force, Eigen::Vector3d torque) {
	SpaceObject::initPhysics(mass, linearVelocity, angularVelocity, force, torque);

    _momentOfInertia = mass * _momentOfInertia;
}
