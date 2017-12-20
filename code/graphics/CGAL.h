/**
 * \brief Keeps all definitions for CGAL.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-11-26
 */

#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel              K;
typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3>        Polyhedron_3;
typedef K::Segment_3                                                   Segment_3;
typedef K::Point_3                                                     Point_3;

typedef Polyhedron_3::Vertex_iterator                                  Vertex_iterator;
typedef Polyhedron_3::Facet_iterator                                   Facet_iterator;
typedef Polyhedron_3::Halfedge_around_facet_circulator                 Halfedge_around_facet_circulator;
