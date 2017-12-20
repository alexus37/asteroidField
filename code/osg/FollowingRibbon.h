/**
 * \brief Functionality for representing the sky-box.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#pragma once

#include <osg/Transform>
#include <osg/Geometry>


namespace pbs17 {

	/**
	 * \brief The SkyBox representate the implementation for a cubemap for the sky visualisation.
	 * This class generates a box and applies the 6 different textures in a way that the user has the feeling to watch on a common environment.
	 */
	class FollowingRibbon {
	public:

		/**
		 *\brief Default constructor. When this is used, don't forget to set up the textures with CSkyBox::setEnvironmentMap.
		 */
		FollowingRibbon();



		/**
		 * \brief Initialize the skybox with the correct openGL commands.
		 */
		osg::Geometry* init(osg::Vec3 startPosition, osg::Vec3 color, unsigned int numPoints, float halfWidth);

		unsigned int getNumPoints() const {
			return _numPoints;
		}

		float getHalfWidth() const {
			return _halfWidth;
		}


	private:

		unsigned int _numPoints = 800;
		float _halfWidth = 0.5f;

		osg::ref_ptr<osg::Vec3Array> _vertices;
		osg::ref_ptr<osg::Vec3Array> _normals;
		osg::ref_ptr<osg::Vec4Array> _colors;
		osg::Vec3 _origin = osg::Vec3(0.0f, 0.0f, 0.0f);
		osg::Vec3 _normal = osg::Vec3(0.0f, 0.0f, 1.0f);
	};
}
