//
//  raytracer.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/29/21.
//

#include "rt_raytracer.hpp"

#include "rt_math.hpp"
#include "rt_geo.hpp"
#include "rt_str.hpp"
#include "rt_vector.hpp"
#include "rt_log.hpp"


std::vector<rt::Vector> rt::RayTracer::trace(rt::Ray start, unsigned int depth) {
    if (depth == 0) {
        return {};
    }
    
    std::vector<rt::Vector> tracedPaths;
    
    Point intersectionPoint;
    LineSegment intersectedEdge;
    bool intersection = this->environment.geometry.intersection(start, intersectionPoint, intersectedEdge);
    
    if (intersection) {
        rt::Vector forwardTrace = rt::Vector(start.origin, intersectionPoint);
        tracedPaths.push_back(forwardTrace);
        
        // recurse on the reflected ray
        
        // reflect forwardTrace vector over intersectedEdge
        rt::Ray reflectedRay = rt::geo::reflect(forwardTrace, intersectedEdge);
        
        auto subTraces = this->trace(reflectedRay, depth - 1);
        
        for (auto trace : subTraces) {
            tracedPaths.push_back(trace);
        }
    }
    
    return tracedPaths;
}


std::vector<std::vector<rt::Vector>> rt::RayTracer::cast(rt::Ray start, float spreadRadians, unsigned int spreadCount, unsigned int depth) {
    std::vector<std::vector<rt::Vector>> casts;
    
    float startRadians = start.radians();
    
    for (int i = 0; i < spreadCount; i++) {
        float radianOffset = i * ((spreadRadians) / ((float) (spreadCount - 1)));
        
        // Offset further if the number of rays is even
        if (spreadCount % 2 == 0) {
            radianOffset -= (spreadRadians / 2.f);
        }
        
        float rayOffset = startRadians + radianOffset;
        
        rt::Ray newStart = Ray(start.origin, rayOffset);
        
        std::vector<rt::Vector> traces = this->trace(newStart, depth);
        
        casts.push_back(traces);
    }
    
    return casts;
}

