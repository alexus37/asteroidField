/**
 * \brief Implementation for the keyboard-interaction in the game-mode.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-05
 */

#include "GameKeyboardHandler.h"

#include "../../physics/SimulationManager.h"
#include "../../scene/SpaceShip.h"


using namespace pbs17;
	
bool GameKeyboardHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) {
	switch (ea.getEventType()) {
	case osgGA::GUIEventAdapter::KEYDOWN:
	{
		switch (ea.getKey()) {
		case osgGA::GUIEventAdapter::KEY_B:
		{
			_showBoundingBox = !_showBoundingBox;

			for (auto it = _objects.begin(); it != _objects.end(); ++it) {
				osg::ref_ptr<osg::Switch> bbSwitch = (*it)->getModel();
				bbSwitch->setValue(1, _showBoundingBox);
			}

			return true;

		}
		case osgGA::GUIEventAdapter::KEY_C:
		{
			_showConvexHull = !_showConvexHull;

			for (auto it = _objects.begin(); it != _objects.end(); ++it) {
				osg::ref_ptr<osg::Switch> bbSwitch = (*it)->getConvexSwitch();
				bbSwitch->setValue(0, !_showConvexHull);
				bbSwitch->setValue(1, _showConvexHull);
			}

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_P:
		{
			_isPaused = !_isPaused;

			SimulationManager::setIsPaused(_isPaused);

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_Plus:
		case osgGA::GUIEventAdapter::KEY_KP_Add:
		{
			SimulationManager::increaseSimulationDt(0.01);

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_Minus:
		case osgGA::GUIEventAdapter::KEY_KP_Subtract:
		{
			SimulationManager::decreaseSimulationDt(0.01);

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_Up:
		case osgGA::GUIEventAdapter::KEY_KP_Up:
		{
			_player->turnUp();

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_Down:
		case osgGA::GUIEventAdapter::KEY_KP_Down:
		{
			_player->turnDown();

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_Left:
		case osgGA::GUIEventAdapter::KEY_KP_Left:
		{
			_player->turnLeft();

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_Right:
		case osgGA::GUIEventAdapter::KEY_KP_Right:
		{
			_player->turnRight();

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_W:
		{
			_player->accelerate();

			return true;
		}
		case osgGA::GUIEventAdapter::KEY_S:
		{
			_player->decelerate();

			return true;
		}
		default:
			return false;
		}
	}

	default:
		return false;
	}
}