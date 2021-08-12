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

rt::Trace* rt::RayTracer::trace(rt::Ray start, unsigned int depth) {

    // std::cout << start << " (" << depth << ")" << std::endl;

    if (depth == 0) {
        return nullptr;
    }

    // std::cout << "depth > 0" << std::endl;

    rt::Trace* startTrace = nullptr;
    
    Point intersectionPoint;
    LineSegment intersectedEdge;
    bool intersection = this->environment.geometry.intersection(start, intersectionPoint, intersectedEdge);
    
    if (intersection) {

        // std::cout << "Intersection." << std::endl;

        rt::Vector forwardTrace(start.origin, intersectionPoint);
        startTrace = new Trace(forwardTrace, 1.0);
        
        // recurse on the reflected ray
        
        // reflect forwardTrace vector over intersectedEdge's normal
        rt::Ray reflectedRay = rt::geo::reflect(forwardTrace, intersectedEdge);
        
        rt::Trace* subTrace = this->trace(reflectedRay, depth - 1);

        if (subTrace) {
            // std::cout << "Subtrace:" << subTrace->vec << std::endl;
            startTrace->next = subTrace;
        }
    }
    
    return startTrace;
}


std::vector<rt::Trace*> rt::RayTracer::cast(rt::Ray start, float spreadRadians, unsigned int spreadCount, unsigned int depth) {
    std::vector<rt::Trace*> casts;
    
    float startRadians = start.radians();
    
    for (int i = 0; i < spreadCount; i++) {
        float radianOffset = i * spreadRadians;

        if (spreadCount % 2 == 0) {
            radianOffset /= (spreadCount);
        }
        
        // Offset further if the number of rays is even
        if (spreadCount % 2 == 1) {
            radianOffset /= (spreadRadians / 2.f);
        }
        
        float rayOffset = startRadians + radianOffset;
        
        rt::Ray newStart(start.origin, rayOffset);
        
        rt::Trace* trace = this->trace(newStart, depth);

        if (trace != nullptr) {
            casts.push_back(trace);
        }
    }

    // std::cout << casts[0] << std::endl;
    
    return casts;
}

