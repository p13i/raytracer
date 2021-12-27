#ifndef rt_environment_hpp
#define rt_environment_hpp

#include "rt_geometry.hpp"

#include <utility>
#include <vector>

using namespace std;
using namespace rt;

namespace rt {

    struct Environment {
        Geometry mGeometry;

        Environment(Geometry geo) :
                mGeometry(std::move(geo)) {}
    };

}

#endif /* rt_environment_hpp */
