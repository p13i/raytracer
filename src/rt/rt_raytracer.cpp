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
#include "rt_linesegment.hpp"

#include <set>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
using namespace rt;

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
    
    float radianOffset = -(spreadRadians / 2.f);
    for (int i = 0; i < spreadCount; i++) {
        
        radianOffset += (spreadRadians / (float) spreadCount);

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


std::vector<rt::Beam*> RayTracer::beamCast(Ray start, float spreadRadians, unsigned int depth) {
    
    const Point o = start.origin;
    
    // Brute force algorithm:
    // 1. Get all unique points in the geometry
    
    vector<Point> allPoints;
    for (LineSegment edge : environment.geometry.edges) {
        allPoints.push_back(edge.a);
        allPoints.push_back(edge.b);
    }
    
    // 2. Create lines from the start to each point
    set<Point> allVisiblePointsSet;
    for (Point point : allPoints) {
        
        // Only add lines that do not intersect with the geometry
        
        Ray intersectionStart(start.origin, point);
        Point intersectionPoint;
        LineSegment intersectedEdge;
        bool intersection = environment.geometry.intersection(intersectionStart, intersectionPoint, intersectedEdge);

        if (intersection) {
            bool intersectA = intersectionPoint.fequal(intersectedEdge.a);
            bool intersectB = intersectionPoint.fequal(intersectedEdge.b);

            intersectA &= intersectedEdge.a.fequal(point);
            intersectB &= intersectedEdge.b.fequal(point);
            
            if (intersectA || intersectB) {
                intersection = false;
            }
        }
        
        if (!intersection) {
            allVisiblePointsSet.insert(point);
        }
    }
    
    if (allVisiblePointsSet.size() < 2) {
        return {};
    }
    
    // 3. Convert visible points set to a sorted list
    vector<Point> visiblePoints;
    for (Point point : allVisiblePointsSet) {
        visiblePoints.push_back(point);
    }
    
    // 3.a Sort all the points by their distance from the start's origin
    sort(visiblePoints.begin(), visiblePoints.end(), [o](const Point& a, const Point& b) -> bool {
        float da = Vector(o, a).magnitude();
        float db = Vector(o, b).magnitude();
        return da < db;
    });
    
    // 3.b Sort all the lines by their radians angle
    sort(visiblePoints.begin(), visiblePoints.end(), [o](const Point& a, const Point& b) -> bool {
        float ra = Ray(o, a).radians();
        float rb = Ray(o, b).radians();
        return ra < rb;
    });
    
    // 4. Collect all line segments into their own Beams
    vector<Beam*> beams;
    int nPoints = visiblePoints.size();
    for (int i = 0; i < nPoints - 1; i++) {
        int i1 = i;
        int i2 = i + 1;
        
        Point p1 = visiblePoints[i1];
        Point p2 = visiblePoints[i2];
        
        Vector boundA(o, p1);
        Vector boundB(o, p2);
        
        Beam *beam = new Beam(boundA, boundB);
        
        beams.push_back(beam);
    }
    
    if (nPoints > 2) {
        Point p1 = visiblePoints[nPoints - 1];
        Point p2 = visiblePoints[0];

        Beam *beam = new Beam({o, p1}, {o, p2});

        beams.push_back(beam);
    }
    
//    for (LineSegment line : allLines) {
//
//        Vector boundA(line.a, line.b);
//        Vector boundB(line.a, line.b);
//
//        boundB.dest = boundB.dest + rt::geo::normal(line).unit().dest * 10;
//
//        Beam* beam = new Beam(boundA, boundB);
//        beams.push_back(beam);
//    }
    
    return beams;
    
    // 3. Collect only the lines that do not intersect in the geometry
//    vector<LineSegment> nonIntersectingLines;
//    for (LineSegment line : allLines) {
//
//        // From start.origin (Step 2) push_back
//        Ray intersectionStart(line.a, line.b);
//        Point intersectionPoint;
//        LineSegment intersectedEdge;
//        bool intersection = environment.geometry.intersection(intersectionStart, intersectionPoint, intersectedEdge);
//
//
//    }

//
//
//
//    Ray intersectionStart(start.origin, allPoints[0]);
//    Point intersectionPoint;
//    LineSegment intersectedEdge;
//    bool intersected = environment.geometry.intersection(intersectionStart, intersectionPoint, intersectedEdge);
//
//    if (intersectionPoint == allPoints[0]) {
//        intersected = false;
//    }
    
    // Convert to beams
//    vector<Beam*> beams;
//    for (int i = 0; i <= allPoints.size() >> 7; i++) {
//        int i1 = i % allPoints.size();
//        int i2 = (i + 1) % allPoints.size();
//        beams.push_back(new Beam(Vector(start.origin, allPoints[i1]), Vector{start.origin, allPoints[i2]}));
//    }
//    return {beams[0]};
}
