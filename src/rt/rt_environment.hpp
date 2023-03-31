#ifndef RT_ENVIRONMENT_HPP
#define RT_ENVIRONMENT_HPP

#include <utility>

#include "rt_geometry.hpp"

namespace rt {

using namespace std;

// Defines components of a scene's environment which currently only includes geometry
struct Environment {
  Geometry mGeometry;

  Environment(Geometry geo)
      : mGeometry(std::move(geo)) {}
};

}  // namespace rt

#endif // RT_ENVIRONMENT_HPP
