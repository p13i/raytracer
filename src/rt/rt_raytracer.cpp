#include "rt_raytracer.hpp"

#include <algorithm>
#include <functional>
#include <set>
#include <vector>

#include "rt_geo.hpp"
#include "rt_linesegment.hpp"
#include "rt_math.hpp"
#include "rt_str.hpp"
#include "rt_vector.hpp"

using namespace std;
using namespace rt;

Trace<Vector>* RayTracer::trace(Ray start, unsigned int depth) {
  // std::cout << start << " (" << depth << ")" << std::endl;

  if (depth == 0) {
    return nullptr;
  }

  // std::cout << "depth > 0" << std::endl;

  Trace<Vector>* startTrace = nullptr;

  Point intersectionPoint;
  LineSegment intersectedEdge;
  bool intersection = this->mEnvironment.mGeometry.intersection(
      start, intersectionPoint, intersectedEdge);

  if (intersection) {
    // std::cout << "Intersection." << std::endl;

    Vector forwardTrace(start.origin, intersectionPoint);
    startTrace = new Trace<Vector>(forwardTrace, 1.0);

    // recurse on the reflected ray

    // reflect forwardTrace vector over intersectedEdge's normal
    Ray reflectedRay = geo::reflect(forwardTrace, intersectedEdge);

    Trace<Vector>* subTrace = this->trace(reflectedRay, depth - 1);

    if (subTrace) {
      // std::cout << "Subtrace:" << subTrace->vec << std::endl;
      startTrace->next = subTrace;
    }
  }

  return startTrace;
}

std::vector<Trace<Vector>*> RayTracer::cast(Ray start, float spreadRadians,
                                            unsigned int spreadCount,
                                            unsigned int depth) {
  std::vector<Trace<Vector>*> casts;

  float startRadians = start.radians();

  float radianOffset = -(spreadRadians / 2.f);
  for (unsigned int i = 0; i < spreadCount; i++) {
    radianOffset += (spreadRadians / static_cast<float>(spreadCount));

    float rayOffset = startRadians + radianOffset;

    Ray newStart(start.origin, rayOffset);

    Trace<Vector>* trace = this->trace(newStart, depth);

    if (trace != nullptr) {
      casts.push_back(trace);
    }
  }

  // std::cout << casts[0] << std::endl;

  return casts;
}

std::vector<Trace<Beam*>*> RayTracer::beamCast(Ray start, float spreadRadians,
                                               unsigned int depth) {
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
    bool intersection = mEnvironment.mGeometry.intersection(
        intersectionStart, intersectionPoint, intersectedEdge);

    if (intersection) {
      bool intersectA = (intersectionPoint == intersectedEdge.a) &&
                        (intersectedEdge.a == point);
      bool intersectB = (intersectionPoint == intersectedEdge.b) &&
                        (intersectedEdge.b == point);

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
  sort(visiblePoints.begin(), visiblePoints.end(),
       [o](const Point& a, const Point& b) -> bool {
         float da = Vector(o, a).magnitude();
         float db = Vector(o, b).magnitude();
         return da < db;
       });

  // 3.b Sort all the lines by their radians angle
  sort(visiblePoints.begin(), visiblePoints.end(),
       [o](const Point& a, const Point& b) -> bool {
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

    Beam* beam = new Beam(bound_a, bound_b);

    beams.push_back(beam);
  }

  if (num_points > 2) {
    Point p1 = visiblePoints[num_points - 1];
    Point p2 = visiblePoints[0];

    Ray r1(o, p1);
    Ray r2(o, p2);

    if ((r1.radians() - r2.radians()) > (RT_PI / 2.f)) {
      Beam* beam = new Beam({o, p1}, {o, p2});
      beams.push_back(beam);
    }
  }

  size_t i = 0;
  while (i < beams.size()) {
    Beam* beam = beams[i];
    Vector boundA = beam->mBoundA;
    Vector boundB = beam->mBoundB;

    LineSegment endOfBoundsOuterSegment{boundA.dest, boundB.dest};

    bool shouldRemove = false;

    for (Beam* otherBeam : beams) {
      Vector otherBeamBoundA = otherBeam->mBoundA;
      Vector otherBeamBoundB = otherBeam->mBoundB;

      Point intersectionPoint;
      if (geo::intersection(endOfBoundsOuterSegment,
                            otherBeamBoundA.lineSegment(), intersectionPoint) &&
          intersectionPoint != otherBeamBoundA.origin &&
          intersectionPoint != otherBeamBoundA.dest) {
        shouldRemove = true;
      }

      if (geo::intersection(endOfBoundsOuterSegment,
                            otherBeamBoundB.lineSegment(), intersectionPoint) &&
          intersectionPoint != otherBeamBoundB.origin &&
          intersectionPoint != otherBeamBoundB.dest) {
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
  for (Beam* beam : beams) {
    Vector boundA = beam->mBoundA, boundB = beam->mBoundB;

    Ray castRayA{boundA.origin, boundA.dest};
    Point intersectionPointA;
    LineSegment intersectedEdgeA;

    bool intersectionA = mEnvironment.mGeometry.intersection(
        castRayA, intersectionPointA, intersectedEdgeA);

    Ray castRayB{boundB.origin, boundB.dest};
    Point intersectionPointB;
    LineSegment intersectedEdgeB;

    bool intersectionB = mEnvironment.mGeometry.intersection(
        castRayA, intersectionPointA, intersectedEdgeA);

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

ProcessUnboundBeamsResult rt::ProcessUnboundBeam(
    const UnboundBeam& u_beam, const std::vector<LineSegment>& env_edges) {
  // Unpack some local variables
  Point b_o = Origin(u_beam);
  Ray b_l = u_beam.bound_a_;
  Ray b_r = u_beam.bound_b_;

  if (b_l.radians() < b_r.radians()) {
    Ray temp = b_l;
    b_l = b_r;
    b_r = temp;
  }

  // Find line segments. Place into w (for :within")
  std::vector<LineSegment> lines_within_u_beam;
  for (const LineSegment& line_segment : env_edges) {
    if (Contains(u_beam, line_segment)) {
      lines_within_u_beam.push_back(line_segment);
    }
  }

  // Step 2: Order w by distance from B_o and select the
  // closest line segment as L_closest
  sort(lines_within_u_beam.begin(), lines_within_u_beam.end(),
       [b_o](const LineSegment& l_a, const LineSegment& l_b) -> bool {
         float da = min(Vector(b_o, l_a.a).magnitude(),
                        Vector(b_o, l_a.b).magnitude());
         float db = min(Vector(b_o, l_b.a).magnitude(),
                        Vector(b_o, l_b.b).magnitude());
         return da < db;
       });

  LineSegment L_closest = lines_within_u_beam.front();

  // Step 3: Trace line segments (actually Vectors) from B_o
  // to the endpoints of L_closest, a_l and a_r, S_1 and S_2
  // We need to find the point on L_closest that is within the
  // bounds of the beam. E.g., if L_closest.a is outside of
  // the beam, then we need to find the point inside
  Vector s_o_a{b_o, L_closest.a};
  Vector s_o_b{b_o, L_closest.b};

  if (Ray(s_o_a).radians() < Ray(s_o_b).radians()) {
    Vector temp = s_o_b;
    s_o_b = s_o_a;
    s_o_a = temp;
  }

  // Step 4: Form a bounded beam from b_o to L_closest(=a).
  // Put this beam into a new list C (completed beams)
  Beam b_beam{s_o_a, s_o_b};
  std::vector<Beam> C{b_beam};

  // Step 5: Remove L_closest from W_ordered
  // lines_within_u_beam.erase(lines_within_u_beam.begin());
  // Done in caller by L_closest_processed being removed

  // Step 6: Construct shadow beams: S_a_l is bounded by B_l
  // and a ray of (B_l spanning to A_l) to infinity, and S_b_r
  // the right side of S_2. Repeat Steps 2 to 6 until no non-
  // nil shadows are formed. C may have more beams now.
  UnboundBeam S_a_l{b_l, s_o_a};
  UnboundBeam S_b_r{b_r, s_o_b};

  // Return result
  ProcessUnboundBeamsResult result;
  result.L_closest_processed = L_closest;
  result.C = C;
  result.next_u_beams = {};
  if (S_a_l.bound_a_ != S_a_l.bound_b_) {
    result.next_u_beams.push_back(S_a_l);
  }
  if (S_b_r.bound_a_ != S_b_r.bound_b_) {
    result.next_u_beams.push_back(S_b_r);
  }

  return result;
}
////////////////////////////////////////////////////////////
///
std::vector<Trace<Beam*>*> RayTracer::beamCast2(Ray start, float spreadRadians,
                                                unsigned int depth) {
  // Step 1: Within the unbounded beam B, find all line
  // segments that are within the beam's bounds
  // (B_l, B_r).

  Point b_o = start.origin;
  Ray b_l = geo::rotate(start, -1 * spreadRadians / 2.f);
  Ray b_r = geo::rotate(start, spreadRadians / 2.f);
  UnboundBeam u_beam{b_l, b_r};

  std::vector<Beam> C;  // completed beams
  std::vector<LineSegment> env_edges = mEnvironment.mGeometry.edges;
  ProcessUnboundBeamsResult result = ProcessUnboundBeam(u_beam, env_edges);

  for (const Beam& beam : result.C) {
    C.push_back(beam);
  }

  // Repeat Steps 2 to 6 goes here
  // Ignore the edge from the result
  std::vector<LineSegment> edges_minus_last_closest;
  std::copy_if(env_edges.begin(), env_edges.end(),
               std::back_inserter(edges_minus_last_closest),
               [result](LineSegment line_seg) {
                 return line_seg != result.L_closest_processed;
               });
  for (const UnboundBeam& next_u_beam : result.next_u_beams) {
    ProcessUnboundBeamsResult next_result =
        ProcessUnboundBeam(next_u_beam, edges_minus_last_closest);
  }

  // Step 7: Reflect each beam in C and repeat Steps 1 to 7
  // until a max depth.

  return {};
}