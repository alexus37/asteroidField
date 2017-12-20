/**
* \brief Functionality for saving each frame to a given location.
*
* \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
* \Date:   2017-12-06
*/

#pragma once

#include <osgDB/WriteFile>
#include <osg/Camera>


namespace pbs17 {

	/**
	 * \brief This callback saves each frame to a given location.
	 */
	class SnapImageDrawCallback : public osg::Camera::DrawCallback {
	public:

		SnapImageDrawCallback()
			: _snapImageOnNextFrame(false) {}

		void setFileName(const std::string& filename) {
			_filename = filename;
		}

		const std::string& getFileName() const {
			return _filename;
		}

		void setSnapImageOnNextFrame(bool flag) {
			_snapImageOnNextFrame = flag;
		}

		bool getSnapImageOnNextFrame() const {
			return _snapImageOnNextFrame;
		}

		void operator () (const osg::Camera& camera) const override;

	protected:

		std::string _filename;
		mutable bool _snapImageOnNextFrame;
	};
}
