/**
 * \brief Calculate the convex-hull of a given object and store all relevant information for further processing.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-29
 */

#include "ConvexHull3D.h"

#include <CGAL/convex_hull_3.h>

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

using namespace pbs17;
namespace SMS = CGAL::Surface_mesh_simplification;


/**
 * \brief Constructor which initializes the convex-hull based on the given vertices.
 *
 * \param vertices
 *      All vertices for which the bounding-box has to be calculated.
 */
ConvexHull3D::ConvexHull3D(osg::Vec3Array* vertices) {
	init(vertices);
}


/**
 * \brief Initialize the convex-hull based on the given vertices.
 *
 * \param vertices
 *      All vertices for which the bounding-box has to be calculated.
 */
void ConvexHull3D::init(osg::Vec3Array* vertices) {
	std::vector<Point_3> points(vertices->size());

	// Transform the osg-vertices into cgal-points
	for (unsigned int i = 0; i < vertices->size(); ++i) {
		osg::Vec3 vertex_at_i = (*vertices)[i];
		points[i] = Point_3(vertex_at_i[0], vertex_at_i[1], vertex_at_i[2]);
	}

	// Define polyhedron to hold convex hull and compute convex hull of non-collinear points
	CGAL::convex_hull_3(points.begin(), points.end(), _cgalModel);
	simplifyCgalModel(_cgalModel, 300);

	fromPolyhedron(_cgalModel, _osgModel, _vertices);
}


/**
 * \brief Simplify the convex-hull model so that it only has the given amount of edges.
 *
 * \param polyhedron
 *		Output-parameter:
 *			Input:		Convex-hull-polyhedron which might be to complex
 *			Output:		Simplified convex-hull-polyhedron which represents the convex-hull to simplify
 * \param numEdges
 *      Number of edges to keep maximally.
 */
void ConvexHull3D::simplifyCgalModel(Polyhedron_3& polyhedron, int numEdges) {
	//std::cout << "\nStart with "
	//	<< (polyhedron.size_of_halfedges() / 2) << "  edges." << std::endl;

	SMS::Count_stop_predicate<Polyhedron_3> stop(numEdges);
	int r = SMS::edge_collapse(polyhedron,
		stop,
		CGAL::parameters::vertex_index_map(get(CGAL::vertex_external_index, polyhedron))
		.halfedge_index_map(get(CGAL::halfedge_external_index, polyhedron))
		.get_cost(SMS::Edge_length_cost <Polyhedron_3>())
		.get_placement(SMS::Midpoint_placement<Polyhedron_3>())
	);

	//std::cout << "\nFinished...\n" << r << " edges removed. "
	//	<< (polyhedron.size_of_halfedges() / 2) << " final edges." << std::endl;
}


/**
 * \brief Convert the polyhedron from CGAL to the geometry from OSG (to render)
 * and to Eigen to calculate further operations (collision-detection etc)
 *
 * \param convexHull
 *      Polyhedron which is the convex-hull (optimally already simplified)
 * \param geometry
 *      Output-parameter:
 *			Input:		ignored
 *			Output:		Geometry for OSG to render the convex-hull
 * \param vertices
 *		Output-parameter:
 *			Input:		ignored
 *			Output:		Eigen-vectors with all the vertices
 */
void ConvexHull3D::fromPolyhedron(Polyhedron_3 &convexHull, osg::ref_ptr<osg::Geometry> &geometry, std::vector<Eigen::Vector3d> &vertices) {
	// Reset output-parameters
	geometry = new osg::Geometry;
	vertices.resize(0);

	// Vectors to store the vertices and faces of the convex-hull
	osg::ref_ptr<osg::Vec3Array> convexVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::DrawElementsUInt> convexFaces = new osg::DrawElementsUInt(GL_TRIANGLES);

	// Set the index of each vertex and add it to the convex-hull-vertices (osg)
	unsigned int index = 0;
	for (Vertex_iterator v = convexHull.vertices_begin(); v != convexHull.vertices_end(); ++v, ++index) {
		v->id() = index;
		Point_3 vertex = v->point();

		convexVertices->push_back(osg::Vec3(CGAL::to_double(vertex[0]), CGAL::to_double(vertex[1]), CGAL::to_double(vertex[2])));
		vertices.push_back(Eigen::Vector3d(CGAL::to_double(vertex[0]), CGAL::to_double(vertex[1]), CGAL::to_double(vertex[2])));
	}

	// Store each face of the convex-hull
	unsigned int row = 0;
	for (Facet_iterator pFacet = convexHull.facets_begin(); pFacet != convexHull.facets_end(); ++pFacet) {
		unsigned int col = 0;
		Halfedge_around_facet_circulator pHalfedge = pFacet->facet_begin();

		do {
			convexFaces->push_back(pHalfedge->vertex()->id());
			++col;
		} while (++pHalfedge != pFacet->facet_begin());

		++row;
	}

	// Create the osg-entity which represents the convex-hull
	geometry->setVertexArray(convexVertices);
	geometry->addPrimitiveSet(convexFaces);
}
