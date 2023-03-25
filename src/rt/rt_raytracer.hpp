#ifndef RT_RAYTRACER_HPP
#define RT_RAYTRACER_HPP

#include <utility>
#include <vector>

#include "rt_beam.hpp"
#include "rt_environment.hpp"
#include "rt_geometry.hpp"
#include "rt_ray.hpp"
#include "rt_trace.hpp"
#include "rt_unbound_beam.hpp"
#include "rt_vector.hpp"

namespace rt {

////////////////////////////////////////////////////////////
/// Encapsulates the core algorithm of a "beam caster"
/// Based in large part by Hanrahan et al.'s 1984 paper
/// "Beam Tracing Polygonal Objects". This class (and the
/// encompassing project are miss named); this should be
/// called a beam caster or beam tracer, not a ray tracer.
class RayTracer {
 public:
  // The scene
  Environment mEnvironment;
  // Default constructor
  RayTracer() : RayTracer({{}}) {}
  // Main constructor
  RayTracer(Environment environment) : mEnvironment(std::move(environment)) {}
  Trace<Vector>* trace(Ray start, unsigned int depth = 1);
  vector<Trace<Vector>*> cast(Ray start, float spreadRadians,
                              unsigned int spreadCount, unsigned int depth = 1);
  vector<Trace<Beam*>*> beamCast(Ray start, float spreadRadians,
                                 unsigned int depth = 1);
  vector<Trace<Beam*>*> beamCast2(Ray start, float spreadRadians,
                                  unsigned int depth = 1);
};

// Helper functions ////////////////////////////////////////
struct ProcessUnboundBeamsResult {
  // The edge of that formed these beams
  LineSegment L_closest_processed;
  std::vector<Beam> C;                    // completed beams
  std::vector<UnboundBeam> next_u_beams;  // resulting from function
};

ProcessUnboundBeamsResult ProcessUnboundBeam(
    const UnboundBeam& u_beam, const std::vector<LineSegment>& env_edges);
}  // namespace rt

#endif  // RT_RAYTRACER_HPP
