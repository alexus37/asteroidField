/**
 * \brief Functionality for representing the sky-box.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-12
 */

#include "SnapImageDrawCallback.h"

#include <osgDB/WriteFile>
#include <osg/Camera>
#include "../config.h"

using namespace pbs17;

void SnapImageDrawCallback::operator()(const osg::Camera& camera) const {
	//osg::notify(osg::NOTICE) << "Saved screen image to `"<<_filename << "`" << std::endl;
	if (!_snapImageOnNextFrame) {
		return;
	}

	int x = camera.getViewport()->x();
	int y = camera.getViewport()->y();
	int width = camera.getViewport()->width();
	int height = camera.getViewport()->height();

	osg::ref_ptr<osg::Image> image = new osg::Image;
	image->readPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE);
	std::string filename = SCREENSHOT_PATH + "/" + _filename;

	if (osgDB::writeImageFile(*image, filename)) {
		std::cout << "Saved screen image to `" << filename << "`" << std::endl;
	}

	_snapImageOnNextFrame = false;
}
