#ifndef APP_COLOR_MANAGER_HPP
#define APP_COLOR_MANAGER_HPP

#include <vector>

#include "ofColor.h"
#include "rt_random.hpp"

// Produces random color values in a cycle
class AppColorManager {
 public:
  // Creates a color manager with `count` number colors
  // preset
  explicit AppColorManager(const size_t count) {
    for (size_t i = 0; i < count; i++) {
      colors_.emplace_back(rt::random::randF(),
                           rt::random::randF(),
                           rt::random::randF());
    }
  }

  ofColor nextColor() {
    ofColor color = colors_[current_color_index_];
    current_color_index_ =
        (current_color_index_ + 1) % colors_.size();
    return color;
  }

  void reset() { current_color_index_ = 0; }

 private:
  std::vector<ofColor> colors_;
  size_t current_color_index_ = 0;
};

#endif  // APP_COLOR_MANAGER_HPP