/**
 * \brief Implementation of the space ship.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-12
 */

#pragma once

#include "SpaceObject.h"

using json = nlohmann::json;
namespace pbs17 {

	/**
	 * \brief Class which represents any space-object with all relevant information needed for calculations
	 */
	class SpaceShip : public SpaceObject {
	public:
		/**
		 * \brief Constructor of SpaceShip.
		 */
		explicit SpaceShip();


		/**
		 * \brief Constructor of SpaceShip with JSON-configuration.
		 *
		 * \param j
		 *      JSON-configuration for the spaceship.
		 */
		explicit SpaceShip(json j);


		/**
		 * \brief Destructor of SpaceShip.
		 */
		~SpaceShip();


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
		void initOsg(Eigen::Vector3d position, double ratio, double scaling) override;


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
		void initPhysics(double mass, Eigen::Vector3d linearVelocity, Eigen::Vector3d angularVelocity, Eigen::Vector3d force, Eigen::Vector3d torque) override;


		/**
		 * \brief Get the node which is used for correctly tracking the model.
		 * 
		 * \return Node which can be used as a tracking-information.
		 */
		osg::ref_ptr<osg::Node> getTrackingNode() const {
			return _modelFile;
		}


		/**
		 * \brief Update the position and orientation of the space-object.
		 *
		 * \param newPosition
		 *      New position of the object.
		 * \param newOrientation
		 *      New orientation of the object.
		 */
		void updatePositionOrientation(Eigen::Vector3d newPosition, osg::Quat newOrientation) override;


		/**
		 * \brief Update the direction and orientation of the space-ship.
		 * 
		 * \param v
		 *      New direction of the object.
		 * \param newOrientation
		 *      New orientation of the object.
		 */
		void updateDirectionOrientation(Eigen::Vector3d v, osg::Quat newOrientation);


		/**
		 * \brief Navigate the space-ship up-wards.
		 */
		void turnUp();


		/**
		 * \brief Navigate the space-ship down-wards.
		 */
		void turnDown();


		/**
		 * \brief Navigate the space-ship left.
		 */
		void turnLeft();


		/**
		 * \brief Navigate the space-ship right.
		 */
		void turnRight();


		/**
		 * \brief Accelerate the space-ship.
		 */
		void accelerate();


		/**
		 * \brief Slow down the space-ship.
		 */
		void decelerate();

	private:

		//! Root-node for the particles
		osg::ref_ptr<osg::MatrixTransform> _particleRoot;

		//! Acceleration-factor
		double _acceleration = 1.2;
		//! Deceleration-factor
		double _decelerate = 0.8;
		//! Rotation-angle
		double _rotationAngle = 0.1; // randians ~ 5.7 degrees
		// Intensity
		double intensity = 1.0;
	};

}
