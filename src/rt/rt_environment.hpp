//
//  rt_environment.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/6/21.
//

#ifndef rt_environment_hpp
#define rt_environment_hpp

#include "rt_geometry.hpp"
#include "rt_polygon.hpp"

#include <vector>

using namespace std;
using namespace rt;

namespace rt {

struct Environment {
    Geometry geometry;
    Environment(): Environment(Geometry()) { }
    Environment(Geometry geo): geometry(geo) { }
    vector<Polygon> convexPolygons();
};

}

#endif /* rt_environment_hpp */
