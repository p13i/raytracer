//
//  raytracer.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/29/21.
//

#ifndef raytracer_hpp
#define raytracer_hpp


#include "rt_geometry.hpp"
#include "rt_linesegment.hpp"
#include "rt_vector.hpp"
#include "rt_ray.hpp"
#include "rt_environment.hpp"
#include "rt_hyperparams.hpp"

#include <vector>


namespace rt {

class RayTracer {
public:
    Environment environment;
    RayTracer(): RayTracer(Environment()) { }
    RayTracer(Environment environment): environment(environment) {}
    std::vector<rt::Vector> trace(Ray start, unsigned int depth = 1);
    std::vector<std::vector<Vector>> cast(Ray start, float spreadRadians, unsigned int spreadCount, unsigned int depth = 1);
};

}


#endif /* raytracer_hpp */
