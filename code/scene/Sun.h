/**
 * \brief Implementation of the sun.
 *
 * \Author: Alexander Lelidis (14-907-562), Andreas Emch (08-631-384), Uroš Teši? (17-950-346)
 * \Date:   2017-12-12
 */

#ifndef SUN_H
#define SUN_H

#include "Planet.h"

namespace pbs17 {

    /**
     * \brief Class which represents any space-object with all relevant information needed for calculations
     */
    class Sun : public Planet {
    public:
        /**
         * \brief Constructor of a Sun.
         *
         * \param size
         *      Size of the planet.
         */
        Sun(double size);

        /**
         * \brief Constructor of Planet with JSON-configuration.
         *
         * \param j
         *      JSON-configuration for the planet.
         */
        Sun(json j);


		/**
		 * \brief Add a lightsource to the scene.
		 * 
		 * \param color
		 *      Color of the light.
		 * 
		 * \return Lightsource which can be added to the osg.
		 */
		osg::ref_ptr<osg::LightSource> addLight(osg::Vec4 color);


		/**
		 * \brief Initialize the texture-properties and shader.
		 */
		void initTexturing() override;


    private:
		
		//! Unique light id which is used for OpenGL.
		int _lightId;

		//! Osg-light with all settings
		osg::ref_ptr<osg::LightSource> _light;

		//! Global id counter for the light
		static int LIGHT_ID;

    };

}

#endif // SUN_H

