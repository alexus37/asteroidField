/**
 * \brief Implementation of the planet.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include "SpaceObject.h"

using json = nlohmann::json;


namespace pbs17 {

	/**
	 * \brief Class which represents any space-object with all relevant information needed for calculations
	 */
	class Planet : public SpaceObject {
	public:
		/**
		 * \brief Constructor of Planet.
		 *
		 * \param size
		 *      Size of the planet.
		 */
		Planet(double size);
		
		
		/**
		 * \brief Constructor of Planet with JSON-configuration.
		 *
		 * \param j
		 *      JSON-configuration for the planet.
		 */
        Planet(json j);


		/**
		 * \brief Constructor of Planet.
		 *
		 * \param size
		 *      Size of the planet.
		 * \param textureName
		 *      Relative location to the texture-file. (Relative from the data-directory in the source).
		 */
		Planet(double size, std::string textureName);


		/**
		 * \brief Destructor of Planet.
		 */
		virtual ~Planet();


		/**
		 * \brief Initialize the space-object for OSG.
		 *
		 * \param position
		 *      Initial translation of the object.
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
		 * \brief Get radius of the planet.
		 * 
		 * \return Radius of the planet.
		 */
		double getRadius() const {
			return _radius;
		}


	private:

		//! Radius of the planet
		double _radius;

	};

}
