#ifndef DATA_SCENES_HPP
#define DATA_SCENES_HPP

#include <vector>

#include "rt_environment.hpp"

namespace data {
namespace scenes {

struct Scene {
  rt::Environment environment_;
};

// Sample scene geometries
static std::vector<Scene> SCENES{
    Scene{rt::Environment{
        rt::Geometry{std::vector<rt::LineSegment>{
            {{-100, -100}, {-100, +100}},
            {{-200, -100}, {-200, +100}},
        }}}},
    Scene{rt::Environment{
        rt::Geometry{vector<rt::LineSegment>{
            {{-200, +100}, {-100, +200}},
            {{0, +100}, {+100, +100}},
            {{+100, +100}, {+200, -100}},
            {{+100, -100}, {-100, -200}},
            {{-100, -100}, {-100, 0}},
            {{25, 0}, {25, 50}},
            {{-50, +50}, {-100, 0}},
            {{+50, -50}, {+75, 0}},

            {{-200, +200}, {+200, +200}},
            {{+200, +200}, {+200, -200}},
            {{+200, -200}, {-200, -200}},

            {{-400, -400}, {-400, +400}},
            {{-400, +400}, {+400, +400}},
            {{+400, +400}, {+400, -400}},
            {{+400, -400}, {-400, -400}},

            {{-300, +300}, {+300, +300}},
            {{+300, +300}, {+300, -300}},
            {{+300, -300}, {+300, -300}},
        }}}},
    // More scenes here...
};

}  // namespace scenes
}  // namespace data

#endif  // DATA_SCENES_HPP