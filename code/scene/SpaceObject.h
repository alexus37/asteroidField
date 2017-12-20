/**
 * \brief Implementation of the space object (base class).
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <Eigen/Core>
#include <osg/Switch>
#include <osg/MatrixTransform>
#include <osg/BoundingBox>
#include <osg/ShapeDrawable>
#include <json.hpp>

#include "../osg/visitors/BoundingBoxVisitor.h"
#include "../graphics/ConvexHull3D.h"

using json = nlohmann::json;


namespace pbs17 {

	/**
	 * \brief Class which represents any space-object with all relevant information needed for calculations
	 */
	class SpaceObject {
	public:
		/**
		 * \brief Constructor of SpaceObject.
		 *
		 * \param filename
		 *      Relative location to the object-file. (Relative from the data-directory in the source).
		 */
		SpaceObject(std::string filename, int i);
		SpaceObject(json j);


		/**
		 * \brief Constructor of SpaceObject.
		 *
		 * \param filename
		 *      Relative location to the object-file. (Relative from the data-directory in the source).
		 * \param textureName
		 *      Relative location to the texture-file. (Relative from the data-directory in the source).
		 */
		SpaceObject(std::string filename, std::string textureName);


		/**
		 * \brief Destructor.
		 */
		virtual ~SpaceObject();


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
		virtual void initOsg(Eigen::Vector3d position, double ratio, double scaling) = 0;


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
		virtual void initPhysics(double mass, Eigen::Vector3d linearVelocity, Eigen::Vector3d angularVelocity, Eigen::Vector3d force, Eigen::Vector3d torque);


		/**
		 * \brief Get the osg-node of the space-object.
		 *
		 * \return OSG-node of the space-object.
		 */
		osg::ref_ptr<osg::Switch> getModel() const {
			return _modelRoot;
		}


		/**
		 * \brief Get the osg-node which contains the switch for the real-model and convex-hull-model.
		 * 
		 * \return OSG-node which contains the rendering-model and convex-hull-model.
		 */
		osg::ref_ptr<osg::Switch> getConvexSwitch() const {
			return _convexRenderSwitch;
		}


		/**
		 * \brief Get the matrix-transformation of this model with the rotation/translation.
		 * 
		 * \return Matrix-transformation
		 */
		osg::ref_ptr<osg::MatrixTransform> getTransformation() const {
			return _transformation;
		}


		/**
		 * \brief Get the AABB of the space-object.
		 */
		osg::BoundingBox getAABB() const {
			return _aabbGlobal;
		}


		/**
		 * \brief Get the ID of the object.
		 *
		 * \return Unique identifier of the object.
		 */
		long getId() const {
			return _id;
		}


		/**
		 * \brief Get the mass of the object.
		 * 
		 * \return Mass of the object.
		 */
		double getMass() const {
			return _mass;
		}


		/**
		 * \brief Get the position of the object.
		 * 
		 * \return Position of the object.
		 */
		Eigen::Vector3d getPosition() const {
			return _position;
		}


		/**
		 * \brief Set the position of the object.
		 * 
		 * \param newPosition
		 *	    New position of the object.
		 */
		void setPosition(Eigen::Vector3d newPosition);


		/**
		 * \brief Get the linear velocity of the object.
		 * 
		 * \return Linear velocity of the object.
		 */
		Eigen::Vector3d getLinearVelocity() const {
			return _linearVelocity;
		}


		/**
		 * \brief Set the linear velcoity of the object.
		 * 
		 * \param v
		 *      Linear velocity of the object.
		 */
		void setLinearVelocity(Eigen::Vector3d v) {
			_linearVelocity = v;
		}


		/**
		 * \brief Get the moment of inertia of the object.
		 * 
		 * \return Moment of inertia of the object.
		 */
		Eigen::Matrix3d getMomentOfInertia() const {
			return _momentOfInertia;
		}


		/**
		 * \brief Set the moment of inertia of the object.
		 * 
		 * \param m
		 *      New moment of inertia-matrix of the object.
		 */
		void setMomentOfInertia(Eigen::Matrix3d m) {
			_momentOfInertia = m;
		}


		/**
		 * \brief Get the angular velocity of the object.
		 * 
		 * \return Angular velocity of the object.
		 */
		Eigen::Vector3d getAngularVelocity() const {
			return _angularVelocity;
		}


		/**
		 * \brief Set the angular velocity of the object.
		 * 
		 * \param av
		 *      New angular velocity of the object.
		 */
		void setAngularVelocity(Eigen::Vector3d av) {
			_angularVelocity = av;
		}


		/**
		 * \brief Get the orientation of the object.
		 * 
		 * \return Orientation of the object.
		 */
		osg::Quat getOrientation() const {
			return _orientation;
		}

		/**
		 * \brief Update the position and orientation of the space-object.
		 * 
		 * \param newPosition
		 *      New position of the object.
		 * \param newOrientation
		 *      New orientation of the object.
		 */
        virtual void updatePositionOrientation(Eigen::Vector3d newPosition, osg::Quat newOrientation);
        

		/**
		 * \brief Calculate the AABB for the object for it's current state.
		 * Here, the correct min-max of each vector is considered.
		 */
		void calculateAABB();


		/**
		 * \brief Update the AABB. Here, only the 8 corners of the original AABB are
		 * rotated/translated correctly to speed up the calculateion. (to calculate the
		 * bounding box for each time step correclty over all vertices is too time consuming).
		 */
		void updateAABB();


		/**
		 * \brief Reset the collision state to 0. Usually before each frame.
		 */
		void resetCollisionState();


		/**
		 * \brief Set the collision state of the object.
		 * 
		 * \param c
		 *      New collision-state. Possible values:
		 *          - 0 = no collision
		 *          - 1 = collision possible
		 *          - 2 = collision for sure
		 */
		void setCollisionState(int c);


		/**
		 * \brief Get the convex hull with the correct global-vertex positions.
		 * 
		 * \return List of vertices of the convex-hull in the global-world-space.
		 */
		std::vector<Eigen::Vector3d> getConvexHull() const;


		/**
		 * \brief Initialize the texture-properties and shader.
		 */
		virtual void initTexturing();


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
		void initFollowingRibbon(osg::Vec3 color, unsigned int numPoints, float halfWidth);

	protected:

		//! Filename of the loaded object
		std::string _filename;
		//! Filename of the loaded texture
		std::string _textureName = "";
		//! Filename of the loaded bumpmap
		std::string _bumpmapName = "";

		//! Unique identifier for the object
		long _id;

		//! Root of the model which is used for the scene
		osg::ref_ptr<osg::Switch> _modelRoot;
		osg::ref_ptr<osg::Switch> _convexRenderSwitch;
		osg::ref_ptr<osg::Node> _modelFile;
		osg::ref_ptr<osg::MatrixTransform> _aabbRendering;
		osg::ref_ptr<osg::ShapeDrawable> _aabbShape;
		//! Local-rotation-node for the object
		osg::ref_ptr<osg::MatrixTransform> _transformation;

		//! Scaling ratio
		double _scaling = 1.0;
		//! Position
		Eigen::Vector3d _position;
		//! Orientation
		osg::Quat _orientation;
		//! AABB of the object
		osg::BoundingBox _aabbLocal;
		osg::BoundingBox _aabbGlobal;
		osg::BoundingBox _aabbLocalOrig;
		osg::BoundingBox _aabbGlobalOrig;
		//! ConvexHull of the object
		ConvexHull3D* _convexHull = nullptr;

		//! Mass: unit = kg
		double _mass = 1.0;
		//! Linear velocity : unit = m / s
		Eigen::Vector3d _linearVelocity;
		//! Angular velocity : unit = rad / s
		Eigen::Vector3d _angularVelocity;
		//! Moment of inertia tensor (N.B. It is in the local coordinate system)
		Eigen::Matrix3d _momentOfInertia;
		//! Global force : unit = vector with norm equals to N
		Eigen::Vector3d _force;
		//! Global torque : unit = vector with norm equals to N*m(newton metre)
		Eigen::Vector3d _torque;

		//! Collision state of the object (0 = no collision, 1 = possible collision, 2 = collision for sure)
		int _collisionState = 0;


	private:

		//! Running Id for uniquely identifying the objects.
		static long RunningId;
	};

}
