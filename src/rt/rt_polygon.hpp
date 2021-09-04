//
//  rt_polygon.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 9/2/21.
//

#ifndef rt_polygon_h
#define rt_polygon_h

#include "rt_linesegment.hpp"

#include <vector>

using namespace std;
using namespace rt;

namespace rt {

struct Polygon {
    vector<LineSegment> edges;
    Polygon(vector<LineSegment> edges): edges(edges) { };
};

}


#endif /* rt_polygon_h */
