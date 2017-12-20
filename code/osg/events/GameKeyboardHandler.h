/**
 * \brief Implementation for the keyboard-interaction in the game-mode.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-05
 */

#pragma once

#include "KeyboardHandler.h"


// Forward deklarations
namespace pbs17 {
	class SpaceObject;
	class SpaceShip;
}

namespace pbs17 {

	class GameKeyboardHandler : public KeyboardHandler {
	public:
		GameKeyboardHandler(const std::vector<SpaceObject*> &objects, SpaceShip *player)
			: KeyboardHandler(objects, true, false, false), _player(player) {}

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) override;

	private:

		SpaceShip *_player;
	};
}