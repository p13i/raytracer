//
//  rt_environment.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/6/21.
//

#ifndef rt_environment_hpp
#define rt_environment_hpp

#include "rt_geometry.hpp"

namespace rt {

struct Environment {
    Geometry geometry;
    Environment(): Environment(Geometry()) { }
    Environment(Geometry geo): geometry(geo) { }
};

}

#endif /* rt_environment_hpp */
