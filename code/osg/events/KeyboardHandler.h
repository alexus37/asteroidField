/**
 * \brief Base class to handle keyboard interactions.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-05
 */

#pragma once

#include "osgGA/GUIEventHandler"


// Forward declarations
namespace pbs17 {
	class SpaceObject;
}


namespace pbs17 {

	class KeyboardHandler : public osgGA::GUIEventHandler {
	public:
		KeyboardHandler(const std::vector<SpaceObject*> &objects, bool showBoundingBox, bool showConvexHull, bool isPaused)
			: _objects(objects), _showBoundingBox(showBoundingBox), _showConvexHull(showConvexHull), _isPaused(isPaused) {}

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) override = 0;

		void setObjects(const std::vector<SpaceObject*> &objects) {
			_objects = objects;
		}

	protected:

		std::vector<SpaceObject*> _objects;

		bool _showBoundingBox;
		bool _showConvexHull;
		bool _isPaused;
	};
}