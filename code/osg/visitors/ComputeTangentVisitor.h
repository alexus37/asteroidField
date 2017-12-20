/**
 * \brief Visitor to compute the tangents.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-06
 */

#pragma once

#include <osg/NodeVisitor>

namespace osg {
	class Array;
	class Node;
	class Geode;
	class Geometry;
}


namespace pbs17 {
	/**
	 * \brief Change the color to a given node and subchilds.
	 */
	class ComputeTangentVisitor : public osg::NodeVisitor {
	public:
		void apply(osg::Node& node) override;

		void apply(osg::Geode& node) override;

		void generateTangentArray(osg::Geometry* geom);
	};
}
