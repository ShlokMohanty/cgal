// Copyright (c) 2008 ETH Zurich (Switzerland)
// Copyright (c) 2008-2009 INRIA Sophia-Antipolis (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Camille Wormser, Jane Tournois, Pierre Alliez, Stephane Tayeb

#ifndef CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_RAY_3_DO_INTERSECT_H
#define CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_RAY_3_DO_INTERSECT_H

#include <CGAL/Ray_3.h>
#include <CGAL/Bbox_3.h>

// inspired from http://cag.csail.mit.edu/~amy/papers/box-jgt.pdf

namespace CGAL {

namespace internal {

  template <typename FT>
  inline
  bool
  bbox_ray_do_intersect_aux(const FT& px, const FT& py, const FT& pz,
                 const FT& qx, const FT& qy, const FT& qz,
                 const FT& bxmin, const FT& bymin, const FT& bzmin,
                 const FT& bxmax, const FT& bymax, const FT& bzmax)
  {
    // std::cerr << "\n"
    //           << to_double(px) << " " << to_double(py) << " " << to_double(pz) << "\n" 
    //           << to_double(qx) << " " << to_double(qy) << " " << to_double(qz) << "\n" 
    //           << to_double(bxmin) << " " << to_double(bymin) << " " << to_double(bzmin) << "\n" 
    //           << to_double(bxmax) << " " << to_double(bymax) << " " << to_double(bzmax) << "\n";

    // (px, py, pz) is the source
    // -----------------------------------
    // treat x coord
    // -----------------------------------
    FT dmin, tmin, tmax;
    if ( qx >= px )
    {
      tmin = bxmin - px;
      tmax = bxmax - px;
      dmin = qx - px;
      if ( tmax < FT(0) )
        return false;
    }
    else
    {
      tmin = px - bxmax;
      tmax = px - bxmin;
      dmin = px - qx;
      if ( tmax < FT(0) )
        return false;
    }

    // std::cerr << "t1 ";

    FT dmax = dmin;
    if ( tmin > FT(0) )
    {
      if ( dmin == FT(0) )
        return false;
    }
    else
    {
      // std::cerr << "t2 ";
      tmin = FT(0);
      dmin = FT(1);
    }

    // -----------------------------------
    // treat y coord
    // -----------------------------------
    FT d_, tmin_, tmax_;
    if ( qy >= py )
    {
      tmin_ = bymin - py;
      tmax_ = bymax - py;
      d_ = qy - py;
    }
    else
    {
      tmin_ = py - bymax;
      tmax_ = py - bymin;
      d_ = py - qy;
    }

    // std::cerr << to_double(dmin) << " " << to_double(tmax_) << " " << to_double(d_) << " "
    //           << to_double(tmin) << " " << to_double(dmax) << " " << to_double(tmin_) << std::endl;

    if ( (dmin*tmax_) < (d_*tmin) || (dmax*tmin_) > (d_*tmax) )
      return false;

    // std::cerr << "t3 ";

    if( (dmin*tmin_) > (d_*tmin) )
    {
      tmin = tmin_;
      dmin = d_;
      // std::cerr << "t4 ";
    }

    if( (dmax*tmax_) < (d_*tmax) )
    {
      tmax = tmax_;
      dmax = d_;
      // std::cerr << "t5 ";
    }

    // -----------------------------------
    // treat z coord
    // -----------------------------------
    if ( qz >= pz )
    {
      tmin_ = bzmin - pz;
      tmax_ = bzmax - pz;
      d_ = qz - pz;
    }
    else
    {
      tmin_ = pz - bzmax;
      tmax_ = pz - bzmin;
      d_ = pz - qz;
    }

    if ( (dmin*tmax_) >= (d_*tmin) && (dmax*tmin_) <= (d_*tmax) ) {
      // std::cerr << "t6";
      return true;
    }
    else {
      // std::cerr << "f6";
      return false;
    }
  }

  template <class K>
  bool do_intersect(const typename K::Ray_3& ray,
                    const CGAL::Bbox_3& bbox,
                    const K&)
  {
    typedef typename K::FT FT;
    typedef typename K::Point_3 Point_3;

    const Point_3& source = ray.source();
    const Point_3& point_on_ray = ray.second_point(); // = point(1);

    return bbox_ray_do_intersect_aux(
                          source.x(), source.y(), source.z(),
                          point_on_ray.x(), point_on_ray.y(), point_on_ray.z(),
                          FT(bbox.xmin()), FT(bbox.ymin()), FT(bbox.zmin()),
                          FT(bbox.xmax()), FT(bbox.ymax()), FT(bbox.zmax()) );
  }

} // namespace internal

template <class K>
bool do_intersect(const CGAL::Ray_3<K>& ray,
		  const CGAL::Bbox_3& bbox)
{
  return typename K::Do_intersect_3()(ray, bbox);
}

template <class K>
bool do_intersect(const CGAL::Bbox_3& bbox,
		  const CGAL::Ray_3<K>& ray)
{
  return typename K::Do_intersect_3()(ray, bbox);
}

} //namespace CGAL

#endif  // CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_RAY_3_DO_INTERSECT_H
