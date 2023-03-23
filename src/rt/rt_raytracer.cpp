#include "rt_raytracer.hpp"

#include "rt_math.hpp"
#include "rt_geo.hpp"
#include "rt_str.hpp"
#include "rt_vector.hpp"
#include "rt_linesegment.hpp"

#include <set>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
using namespace rt;

rt::Trace<Vector>* rt::RayTracer::trace(rt::Ray start, unsigned int depth) {

    // std::cout << start << " (" << depth << ")" << std::endl;

    if (depth == 0) {
        return nullptr;
    }

    // std::cout << "depth > 0" << std::endl;

    rt::Trace<Vector>* startTrace = nullptr;
    
    Point intersectionPoint;
    LineSegment intersectedEdge;
    bool intersection = this->mEnvironment.mGeometry.intersection(start, intersectionPoint, intersectedEdge);
    
    if (intersection) {

        // std::cout << "Intersection." << std::endl;

        rt::Vector forwardTrace(start.origin, intersectionPoint);
        startTrace = new Trace<Vector>(forwardTrace, 1.0);
        
        // recurse on the reflected ray
        
        // reflect forwardTrace vector over intersectedEdge's normal
        rt::Ray reflectedRay = rt::geo::reflect(forwardTrace, intersectedEdge);
        
        rt::Trace<Vector>* subTrace = this->trace(reflectedRay, depth - 1);

        if (subTrace) {
            // std::cout << "Subtrace:" << subTrace->vec << std::endl;
            startTrace->next = subTrace;
        }
    }
    
    return startTrace;
}


std::vector<rt::Trace<Vector>*> rt::RayTracer::cast(rt::Ray start, float spreadRadians, unsigned int spreadCount, unsigned int depth) {
    std::vector<rt::Trace<Vector>*> casts;
    
    float startRadians = start.radians();
    
    float radianOffset = -(spreadRadians / 2.f);
    for (int i = 0; i < spreadCount; i++) {
        
        radianOffset += (spreadRadians / (float) spreadCount);

        float rayOffset = startRadians + radianOffset;
        
        rt::Ray newStart(start.origin, rayOffset);
        
        rt::Trace<Vector>* trace = this->trace(newStart, depth);

        if (trace != nullptr) {
            casts.push_back(trace);
        }
    }

    // std::cout << casts[0] << std::endl;
    
    return casts;
}


std::vector<Trace<Beam*>*> RayTracer::beamCast(Ray start, float spreadRadians, unsigned int depth) {
    // Recursion base case
    if (depth == 0) {
        return {};
    }
    
    const Point o = start.origin;
    
    // Brute force algorithm:
    // 1. Get all unique points in the mGeometry
    vector<Point> allPoints;
    for (LineSegment edge : mEnvironment.mGeometry.edges) {
        allPoints.push_back(edge.a);
        allPoints.push_back(edge.b);
    }
    
    // 2. Create lines from the start to each point
    set<Point> allVisiblePointsSet;
    for (Point point : allPoints) {
        
        // Only add lines that do not intersect with the mGeometry
        
        Ray intersectionStart(start.origin, point);
        Point intersectionPoint;
        LineSegment intersectedEdge;
        bool intersection = mEnvironment.mGeometry.intersection(intersectionStart, intersectionPoint, intersectedEdge);

        if (intersection) {
            bool intersectA = (intersectionPoint == intersectedEdge.a) && (intersectedEdge.a == point);
            bool intersectB = (intersectionPoint == intersectedEdge.b) && (intersectedEdge.b == point);

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
    size_t num_points = visiblePoints.size();
    for (size_t i = 0; i < num_points - 1; i++) {
        size_t i1 = i;
        size_t i2 = i + 1;
        
        Point p1 = visiblePoints[i1];
        Point p2 = visiblePoints[i2];
        
        Vector bound_a(o, p1);
        Vector bound_b(o, p2);
        
        Beam *beam = new Beam(bound_a, bound_b);
        
        beams.push_back(beam);
    }
    
    if (num_points > 2) {
        Point p1 = visiblePoints[num_points - 1];
        Point p2 = visiblePoints[0];
        
        Ray r1(o, p1);
        Ray r2(o, p2);
        
        if ((r1.radians() - r2.radians()) > (RT_PI / 2.f)) {
            Beam *beam = new Beam({o, p1}, {o, p2});
            beams.push_back(beam);
        }
    }
    
    size_t i = 0;
    while (i < beams.size()) {
        Beam *beam = beams[i];
        Vector boundA = beam->mBoundA;
        Vector boundB = beam->mBoundB;
        
        LineSegment endOfBoundsOuterSegment{boundA.dest, boundB.dest};
        
        bool shouldRemove = false;
        
        for (Beam *otherBeam : beams) {
            Vector otherBeamBoundA = otherBeam->mBoundA;
            Vector otherBeamBoundB = otherBeam->mBoundB;
            
            Point intersectionPoint;
            if (rt::geo::intersection(endOfBoundsOuterSegment, otherBeamBoundA.lineSegment(), intersectionPoint)
                && intersectionPoint != otherBeamBoundA.origin && intersectionPoint != otherBeamBoundA.dest) {
                shouldRemove = true;
            }
            
            if (rt::geo::intersection(endOfBoundsOuterSegment, otherBeamBoundB.lineSegment(), intersectionPoint)
                && intersectionPoint != otherBeamBoundB.origin && intersectionPoint != otherBeamBoundB.dest) {
                shouldRemove = true;
            }
        }
        
        if (shouldRemove) {
            beams.erase(beams.begin() + i);
        } else {
            i++;
        }
    }
    
    // Extend the current beams if both endpoints are on different edges
    for (Beam *beam : beams) {
        
        Vector boundA = beam->mBoundA, boundB = beam->mBoundB;
        
        Ray castRayA{boundA.origin, boundA.dest};
        Point intersectionPointA;
        LineSegment intersectedEdgeA;
        
        bool intersectionA = mEnvironment.mGeometry.intersection(castRayA, intersectionPointA, intersectedEdgeA);
        
        Ray castRayB{boundB.origin, boundB.dest};
        Point intersectionPointB;
        LineSegment intersectedEdgeB;
        
        bool intersectionB = mEnvironment.mGeometry.intersection(castRayA, intersectionPointA, intersectedEdgeA);
        
        if (intersectedEdgeA == intersectedEdgeB) {
            // Perform a reflection
        } else {
            // Continue tracing the beam forward on both ranges
            
            // boundA
            
//            Ray extendRayA{intersectionPointA};
            
            // boundB
        }
    }
    
    // Transform the beams to traces and perform the recursion!!
    
    vector<Trace<Beam*>*> beamTraces;
    for (Beam* beam : beams) {
        
        Vector boundA = beam->mBoundA, boundB = beam->mBoundB;
        
        beamTraces.push_back(new Trace<Beam*>(beam, 1.f, false, nullptr));
    }
    
    return beamTraces;
}

////////////////////////////////////////////////////////////
/// 
std::vector<Trace<Beam*>*> RayTracer::beamCast2(Ray start, float spreadRadians, unsigned int depth) {
    // Step 1: Within the unbounded beam B, find all line
    // segments that are within the beam's bounds
    // (B_l, B_r).

    Point b_o = start.origin;
    Ray b_l = rt::geo::rotate(start, -1 * spreadRadians / 2.f);
    Ray b_r = rt::geo::rotate(start, spreadRadians / 2.f);
    
    return {};
}