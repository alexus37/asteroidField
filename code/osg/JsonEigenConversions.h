/**
* \brief Conversions between json- and eigen-datatypes.
*
* \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
* \Date:   2017-11-29
*/

#pragma once

#include <Eigen/Core>
#include <json.hpp>

using json = nlohmann::json;

namespace pbs17 {
	inline Eigen::Vector3d fromJson(json j) {
		return Eigen::Vector3d(
			j["x"].get<double>(),
			j["y"].get<double>(),
			j["z"].get<double>()
		);
	}
}