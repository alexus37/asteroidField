/**
 * \brief Implementation of the n-body simulation and interaction.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-11
 */

#pragma once

#include <Eigen/Core>
#include <vector>
#include <map>

// Forward declarations
namespace pbs17 {
	class SpaceObject;
}

namespace pbs17 {

	/**
	 * \brief The manager to handle the collision (within all needed calculations) to simulate one frame.
	 */
	class NBodyManager {
	public:
		/**
		 * \brief Constructor of the CollisionManager.
		 *
		 */
		NBodyManager();


		/**
		 * \brief Simulate the scene.
		 *
		 * \param dt
		 *      Time difference since between the last frames.
		 * \param spaceObjects
		 *      All space-objects in the scene.
		 */
		void simulateStep(double dt, std::vector<SpaceObject*> &spaceObjects);


		void initSpatialGrid(std::vector<SpaceObject*> &spaceObjects, Eigen::Vector3i gridResolution);

		
	private:
		//CONST
		const double G = 1.0; // 6.67408 * pow(10.0, -4.0);
		const double EPS = 0.000000001;

		//! Flag if the spatial grid is used or not.
		bool _useSpatialGrid = false;

		//! Bounding-box of the spatial-grid
		Eigen::Vector3d _spatialBbMin;
		Eigen::Vector3d _spatialBbMax;

		//! Reoslution size (number of grids) and length of each grid.
		Eigen::Vector3i _resolutionSize;
		Eigen::Vector3d _resolution;;

		//! Treshold used to cut of the influencer.
		double _treshold = 0.1;

		//! Spatial grid which contains the objects per grid
		std::vector<std::vector<SpaceObject*>> _spatialGrid;
		//! Grid-cells with the references of the influencer which have an impact on this cell
		std::vector<std::map<long, SpaceObject*>> _influencer;

		//! Positions in the grid of each object. (row => object, 3d vector of indices per object)
		Eigen::MatrixXi _spatialPosition;

		//! Number of cells in each dimension the objects have influence
		Eigen::MatrixXi _influenceRadius;
		

		/**
		 * \brief Get the position of the point in the spatial-index-grid for a given point.
		 *
		 * \param position Position of the point which one want to know the index in the spatial-grid
		 *
		 * \return Position in the spatial-index
		 */
		Eigen::Vector3i binSpatialInformation(Eigen::Vector3d position);


		/**
		 * \brief Get the position of the point in the spatial-index-grid for a given point.
		 *
		 * \param position Position of the point which one want to know the index in the spatial-grid
		 *
		 * \return Position in the spatial-index
		 */
		unsigned int getIndex(Eigen::Vector3i position);

		/**
		 * \brief Add an influencer on the specified
		 */
		void addInfluencer(unsigned int i, SpaceObject* spaceObject, Eigen::Vector3i position);
		void removeInfluencer(unsigned int i, SpaceObject* spaceObject, Eigen::Vector3i position);

	};

}
