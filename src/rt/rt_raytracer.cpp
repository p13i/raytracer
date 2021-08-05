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
    
    float time = 0.;
    
    bool intersection = false;
    Point intersectionPoint;
    rt::LineSegment intersectedEdge;
    while (!intersection) {
        
        Point marchedRayPoint = start(time);
    
        // Find first intersection via ray marching
        for (rt::LineSegment edge : this->environment.geometry.edges) {
            
            if (rt::geo::intersection(edge, {start.origin, marchedRayPoint}, intersectionPoint)) {
                
                float rayOriginToIxnPointDist = rt::geo::dist(start.origin, intersectionPoint);
                float marchedPtToIxnPointDistance = rt::geo::dist(marchedRayPoint, intersectionPoint);
                
                // Make sure the ray was facing to the intersection point
                if (rayOriginToIxnPointDist > marchedPtToIxnPointDistance) {
                    intersection = true;
                    intersectedEdge = edge;
                    break;
                }
            }
        }
        
        if (!intersection) {
            time += RT_RAY_MARCH_TIME_STEP;
        }
        
        if (time > RT_RAY_MARCH_TIME_LIMIT) {
            break;
        }
    }
    
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
    
    float startRadius = start.radians();
    
    for (int i = 0; i < spreadCount; i++) {
        float radianOffset = spreadRadians * (float) (i / (float) spreadCount);
        
        rt::Ray newStart = Ray(start.origin, startRadius + radianOffset);
        
        std::vector<rt::Vector> traces = this->trace(newStart, depth);
        
        casts.push_back(traces);
    }
    
    return casts;
}

