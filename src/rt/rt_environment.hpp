#ifndef rt_environment_hpp
#define rt_environment_hpp

#include <utility>
#include <vector>

#include "rt_geometry.hpp"

using namespace std;
using namespace rt;

namespace rt {

struct Environment {
  Geometry mGeometry;

  Environment(Geometry geo) : mGeometry(std::move(geo)) {}
};

}  // namespace rt

#endif /* rt_environment_hpp */