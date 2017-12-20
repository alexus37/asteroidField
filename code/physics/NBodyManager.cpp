/**
 * \brief Implementation of the n-body simulation and interaction.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#include "NBodyManager.h"

#include <math.h>

#if defined(_OPENMP)
#include <omp.h>
#endif

#include "../scene/SpaceObject.h"

using namespace pbs17;


NBodyManager::NBodyManager() {}


void NBodyManager::simulateStep(double dt, std::vector<SpaceObject *> &spaceObjects) {
	int cntSpaceObj = spaceObjects.size();

	// initialize the forces
	std::vector<Eigen::Vector3d> forces(cntSpaceObj);
	for (int i = 0; i < cntSpaceObj; ++i) {
		forces[i] = Eigen::Vector3d(0.0, 0.0, 0.0);
	}

	if (_useSpatialGrid) {
		for (int i = 0; i < cntSpaceObj; ++i) {
			SpaceObject* curObject = spaceObjects[i];
			// Calculate a rotation around the rotation-center of the object
			Eigen::Vector3d curCenter = curObject->getPosition();
			double m = curObject->getMass();

			// acceleration vector
			std::vector<SpaceObject*> objectsToIterate;

			Eigen::Vector3i gridPosition = _spatialPosition.row(i);
			unsigned int index = getIndex(gridPosition);

			//for (int j = 0; j < cntSpaceObj; ++j) {
			if (index > 0 && index < _influencer.size()) {
				for (std::map<long, SpaceObject*>::iterator it = _influencer[index].begin(); it != _influencer[index].end(); ++it) {
					SpaceObject* compareObject = (*it).second;

					if (curObject->getId() == compareObject->getId()) continue; // do not compare the object with it self

					// compare the objects based on the center of mass
					Eigen::Vector3d compareCenter = compareObject->getPosition();

					// get the distance
					Eigen::Vector3d d = compareCenter - curCenter;

					// compute the square distance
					double r = (compareCenter - curCenter).norm();
					r *= r;

					// F is the force between the masses
					double f = (G * m * compareObject->getMass()) / (r + EPS);
					forces[i] += f * d.normalized();
				}
			}
		}
	} else {

#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (int i = 0; i < cntSpaceObj; ++i) {
			SpaceObject* curObject = spaceObjects[i];
			// Calculate a rotation around the rotation-center of the object
			Eigen::Vector3d curCenter = curObject->getPosition();
			double m = curObject->getMass();

			// acceleration vector
			std::vector<SpaceObject*> objectsToIterate;


			for (int j = 0; j < cntSpaceObj; ++j) {
				if (i == j) continue; // do not compare the object with it self

									  // compare the objects based on the center of mass
				SpaceObject* compareObject = spaceObjects[j];
				Eigen::Vector3d compareCenter = compareObject->getPosition();

				// get the distance
				Eigen::Vector3d d = compareCenter - curCenter;

				// compute the square distance
				double r = (compareCenter - curCenter).norm();
				r *= r;

				// F is the force between the masses
				double f = (G * m * compareObject->getMass()) / (r + EPS);
				forces[i] += f * d.normalized();
			}
		}
	}


#if defined(_OPENMP)
#pragma omp parallel for
#endif
	// update positions
	for (int i = 0; i < cntSpaceObj; ++i) {
		SpaceObject* spaceObject = spaceObjects[i];
		Eigen::Vector3d a = forces[i] / spaceObject->getMass();
		Eigen::Vector3d v = spaceObject->getLinearVelocity() + (dt * a);
		spaceObject->setLinearVelocity(v);

		Eigen::Vector3d dtv = dt * v;
		Eigen::Vector3d p = spaceObject->getPosition() + dtv;

		Eigen::Vector3d dto = dt * spaceObject->getAngularVelocity();
		osg::Quat q;
		double sinQuat = sin(dto.norm() / 2);
		double cosQuat = cos(dto.norm() / 2);
		if (dto.norm() > 0) {
			q.set(sinQuat*dto(0) / dto.norm(), sinQuat*dto(1) / dto.norm(), sinQuat*dto(2) / dto.norm(), cosQuat);
		} else {
			q.set(0.0, 0.0, 0.0, 1.0);
		}
		osg::Quat newQ = q*spaceObject->getOrientation();

		spaceObject->updatePositionOrientation(p, newQ);

		if (_useSpatialGrid) {
			// Update influencing area
			Eigen::Vector3i oldPosition = _spatialPosition.row(i);
			Eigen::Vector3i newPosition = binSpatialInformation(p);
			Eigen::Vector3i diffPosition = oldPosition - newPosition;

			if (diffPosition.squaredNorm() != 0) {
				removeInfluencer(i, spaceObject, oldPosition);
				addInfluencer(i, spaceObject, newPosition);
			}

			_spatialPosition.row(i) = newPosition;
		}
	}
}



void NBodyManager::initSpatialGrid(std::vector<SpaceObject*> &spaceObjects, Eigen::Vector3i gridResolution) {
	_useSpatialGrid = true;
	_resolutionSize = gridResolution;

	double max = std::numeric_limits<double>::max();
	Eigen::Vector3d spatialBbMax = Eigen::Vector3d(-max, -max, -max);
	Eigen::Vector3d spatialBbMin = Eigen::Vector3d(max, max, max);

	for (std::vector<SpaceObject*>::iterator it = spaceObjects.begin(); it != spaceObjects.end(); ++it) {
		osg::BoundingBox bbBox = (*it)->getAABB();

		spatialBbMax(0) = std::max(spatialBbMax(0), static_cast<double>(bbBox.xMax()));
		spatialBbMax(1) = std::max(spatialBbMax(1), static_cast<double>(bbBox.yMax()));
		spatialBbMax(2) = std::max(spatialBbMax(2), static_cast<double>(bbBox.zMax()));

		spatialBbMin(0) = std::min(spatialBbMin(0), static_cast<double>(bbBox.xMin()));
		spatialBbMin(1) = std::min(spatialBbMin(1), static_cast<double>(bbBox.yMin()));
		spatialBbMin(2) = std::min(spatialBbMin(2), static_cast<double>(bbBox.zMin()));
	}

	Eigen::Vector3d spatialBbCenter = (spatialBbMax + spatialBbMin) * 0.5;
	Eigen::Vector3d centerToMax = (spatialBbMax - spatialBbCenter).cwiseAbs().maxCoeff() * Eigen::Vector3d(1.0, 1.0, 1.0);
	Eigen::Vector3d centerToMin = (spatialBbMin - spatialBbCenter).cwiseAbs().maxCoeff() * Eigen::Vector3d(-1.0, -1.0, -1.0);

	_spatialBbMax = spatialBbCenter + 2 * centerToMax;
	_spatialBbMin = spatialBbCenter + 2 * centerToMin;
	Eigen::Vector3d bbDim = _spatialBbMax - _spatialBbMin;

	_spatialGrid = std::vector<std::vector<SpaceObject*>>(gridResolution(0) * gridResolution(1) * gridResolution(2), std::vector<SpaceObject*>(0));
	_influencer = std::vector<std::map<long, SpaceObject*>>(gridResolution(0) * gridResolution(1) * gridResolution(2));

	_spatialPosition.resize(spaceObjects.size(), 3);
	_influenceRadius.resize(spaceObjects.size(), 3);

	_resolution(0) = bbDim(0) / static_cast<double>(_resolutionSize(0) - 1);
	_resolution(1) = bbDim(1) / static_cast<double>(_resolutionSize(1) - 1);
	_resolution(2) = bbDim(2) / static_cast<double>(_resolutionSize(2) - 1);

	// Bin each point
	for (unsigned int i = 0; i < spaceObjects.size(); ++i) {
		Eigen::Vector3d current = spaceObjects[i]->getPosition();
		Eigen::Vector3i position = binSpatialInformation(current);
		unsigned int index = getIndex(position);

		_spatialPosition.row(i) << position(0), position(1), position(2);
		_spatialGrid[index].push_back(spaceObjects[i]);

		double influenceForce = -(_treshold * EPS - G * spaceObjects[i]->getMass()) / _treshold;
		Eigen::Vector3i influence = Eigen::Vector3i(static_cast<unsigned int>(influenceForce / _resolution(0)),
			static_cast<unsigned int>(influenceForce / _resolution(1)),
			static_cast<unsigned int>(influenceForce / _resolution(2)));
		influence(0) = std::max(influence(0), 1);
		influence(1) = std::max(influence(1), 1);
		influence(2) = std::max(influence(2), 1);
		_influenceRadius.row(i) = influence;

		addInfluencer(i, spaceObjects[i], position);
	}
}



/**
 * \brief Get the position of the point in the spatial-index-grid for a given point.
 *
 * \param position Position of the point which one want to know the index in the spatial-grid
 *
 * \return Position in the spatial-index
 */
Eigen::Vector3i NBodyManager::binSpatialInformation(Eigen::Vector3d position) {
	Eigen::Vector3d current = position - _spatialBbMin;
	unsigned int i_x = std::floor(current(0) / _resolution(0));
	unsigned int i_y = std::floor(current(1) / _resolution(1));
	unsigned int i_z = std::floor(current(2) / _resolution(2));

	return Eigen::Vector3i(i_x, i_y, i_z);
}

unsigned int NBodyManager::getIndex(Eigen::Vector3i position) {
	unsigned int index = position(0) + _resolutionSize(0) * (position(1) + _resolutionSize(1) * position(2));
	return index;
}

void NBodyManager::addInfluencer(unsigned int i, SpaceObject* spaceObject, Eigen::Vector3i position) {
	Eigen::Vector3i influence = _influenceRadius.row(i);

	if (position(0) < 0 || position(1) < 0 || position(2) < 0
		|| position(0) > _resolutionSize(0) || position(1) > _resolutionSize(1) || position(2) >_resolutionSize(2)) {
		return;
	}

	unsigned int x_min = std::max(position(0) - influence(0), 0);
	unsigned int x_max = std::min(position(0) + influence(0), static_cast<int>(_resolutionSize(0) - 1));
	unsigned int y_min = std::max(position(1) - influence(1), 0);
	unsigned int y_max = std::min(position(1) + influence(1), static_cast<int>(_resolutionSize(1) - 1));
	unsigned int z_min = std::max(position(2) - influence(2), 0);
	unsigned int z_max = std::min(position(2) + influence(2), static_cast<int>(_resolutionSize(2) - 1));

	// First look in the same cell as well the direct neighbors
	for (unsigned int i_x = x_min; i_x <= x_max; ++i_x) {
		for (unsigned int i_y = y_min; i_y <= y_max; ++i_y) {
			for (unsigned int i_z = z_min; i_z <= z_max; ++i_z) {
				unsigned int indexInfluence = i_x + _resolutionSize(0) * (i_y + _resolutionSize(1) * i_z);

				if (indexInfluence < _influencer.size()) {
					_influencer[indexInfluence][spaceObject->getId()] = spaceObject;
				}
			}
		}
	}
}

void NBodyManager::removeInfluencer(unsigned int i, SpaceObject* spaceObject, Eigen::Vector3i position) {
	Eigen::Vector3i influence = _influenceRadius.row(i);

	if (position(0) < 0 || position(1) < 0 || position(2) < 0
		|| position(0) > _resolutionSize(0) || position(1) > _resolutionSize(1) || position(2) >_resolutionSize(2)) {
		return;
	}

	unsigned int x_min = std::max(position(0) - influence(0), 0);
	unsigned int x_max = std::min(position(0) + influence(0), static_cast<int>(_resolutionSize(0) - 1));
	unsigned int y_min = std::max(position(1) - influence(1), 0);
	unsigned int y_max = std::min(position(1) + influence(1), static_cast<int>(_resolutionSize(1) - 1));
	unsigned int z_min = std::max(position(2) - influence(2), 0);
	unsigned int z_max = std::min(position(2) + influence(2), static_cast<int>(_resolutionSize(2) - 1));

	// First look in the same cell as well the direct neighbors
	for (unsigned int i_x = x_min; i_x <= x_max; ++i_x) {
		for (unsigned int i_y = y_min; i_y <= y_max; ++i_y) {
			for (unsigned int i_z = z_min; i_z <= z_max; ++i_z) {
				unsigned int indexInfluence = i_x + _resolutionSize(0) * (i_y + _resolutionSize(1) * i_z);

				if (indexInfluence < _influencer.size()) {
					std::map<long, SpaceObject*> fluencerMap = _influencer[indexInfluence];
					std::map<long, SpaceObject*>::iterator iter = fluencerMap.find(spaceObject->getId());
					if (iter != fluencerMap.end()) {
						fluencerMap.erase(spaceObject->getId());
					}
				}
			}
		}
	}
}