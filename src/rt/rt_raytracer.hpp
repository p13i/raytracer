//
//  raytracer.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/29/21.
//

#ifndef raytracer_hpp
#define raytracer_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <cmath>

#include <memory>
#include <string>
#include <stdexcept>

// *****
// Macros
// *****

#define RT_RAY_MARCH_TIME_STEP 0.01
#define RT_RAY_MARCH_TIME_LIMIT 1000.0

#define RT_MAX_DEPTH 3


#include "rt_linesegment.hpp"
#include "rt_vector.hpp"
#include "rt_ray.hpp"


namespace rt {

struct Geometry {
    std::vector<LineSegment> edges;
    Geometry(): Geometry(std::vector<rt::LineSegment>()) { }
    Geometry(std::vector<rt::LineSegment> edges): edges(edges) { }
};

struct Environment {
    Geometry geometry;
    Environment(): Environment(rt::Geometry()) { }
    Environment(rt::Geometry geo): geometry(geo) { }
};

class RayTracer {
public:
    Environment environment;
    RayTracer(): RayTracer(rt::Environment()) { }
    RayTracer(Environment environment): environment(environment) {}
    std::vector<rt::Vector> trace(rt::Ray start, unsigned int depth = 1);
    std::vector<std::vector<Vector>> cast(Ray start, float spreadRadians, unsigned int spreadCount, unsigned int depth = 1);
};

}


#endif /* raytracer_hpp */
