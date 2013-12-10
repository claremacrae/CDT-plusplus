/// Causal Dynamical Triangulations in C++ using CGAL
///
/// Copyright (c) 2013 Adam Getchell
///
/// Spherical simplicial complexes

#ifndef SPHERICAL_3_COMPLEX_H_
#define SPHERICAL_3_COMPLEX_H_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/point_generators_3.h>

/// C++ headers
#include <cassert>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Delaunay_triangulation_3<K>                     Delaunay;

/// Make 3D spherical simplicial complexes
template <typename T>
void make_S3_simplicial_complex(T* S3, int number_of_simplices) {
  CGAL::Random_points_in_sphere_3<typename T::Point> rnd;

  /// Initialize triangulation in 3D
  S3->insert(typename T::Point(0, 0, 0));
  S3->insert(typename T::Point(1, 0, 0));
  S3->insert(typename T::Point(0, 1, 0));
  S3->insert(typename T::Point(0, 0, 1));

  assert(S3->dimension() == 3);

  std::cout  << "Initial seed has " << S3->number_of_vertices()
        << " vertices and " << S3->number_of_facets() << " facets"
        << " and " << S3->number_of_cells() << " cells" << std::endl;


  do {
     Delaunay::Point p = *rnd++;

    /// Locate the point
    typename T::Locate_type lt;
    int li, lj;
    typename T::Cell_handle c = S3->locate(p, lt, li, lj);
    if (lt == Delaunay::VERTEX)
      continue;  // Point already exists

    /// Get the cells that conflict with p in a vector V,
    /// and a facet on the boundary of this hole in f
    std::vector<typename T::Cell_handle> V;
    Delaunay::Facet f;

    S3->find_conflicts(p, c,
              /// Get one boundary facet
              CGAL::Oneset_iterator<Delaunay::Facet>(f),
              /// Conflict cells in V
              std::back_inserter(V));

    if ((V.size() & 1) == 0)  // Even number of conflict cells?
      S3->insert_in_hole(p, V.begin(), V.end(), f.first, f.second);
  } while (S3->number_of_cells() < number_of_simplices);

  assert(S3->dimension() == 3);
  assert(S3->is_valid());
}
#endif  // SPHERICAL_3_COMPLEX_H_