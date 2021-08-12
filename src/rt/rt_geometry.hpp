//
//  rt_geometry.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/6/21.
//

#ifndef rt_geometry_hpp
#define rt_geometry_hpp

#include "rt_linesegment.hpp"
#include "rt_ray.hpp"

#include <vector>

namespace rt {

struct Geometry {
    std::vector<LineSegment> edges;
    Geometry(): Geometry(std::vector<rt::LineSegment>()) { }
    Geometry(std::vector<rt::LineSegment> edges): edges(edges) { }
    bool intersection(Ray start, Point& intersectionPoint, LineSegment& intersectedEdge);
};

}

#endif /* rt_geometry_hpp */
