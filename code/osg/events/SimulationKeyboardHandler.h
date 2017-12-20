/**
 * \brief Implementation for the keyboard-interaction in the simulation-mode.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-05
 */

#pragma once

#include "KeyboardHandler.h"


namespace pbs17 {
	class SimulationKeyboardHandler : public KeyboardHandler {
	public:
		SimulationKeyboardHandler(const std::vector<SpaceObject*> &objects)
			: KeyboardHandler(objects, true, false, false) {}

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) override;

	};
}
