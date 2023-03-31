#ifndef APP_PURE_TONE_HPP
#define APP_PURE_TONE_HPP

#include <vector>

#include "rt_math.hpp"

// Represents one audio frequency, such as A 440 Hz
class PureTone {
 public:
  explicit PureTone(const float frequency)
      : frequency_hz_(frequency) {}

  // Writes the frequency to a new buffer
  std::vector<float> asBuffer(size_t length) const {
    std::vector<float> buffer;
    // Allocate the array first, more efficient
    buffer.assign(length, 0.f);
    for (size_t i = 0; i < length; i++) {
      // Phase is the duration into a sine wave as a
      // fraction of second
      float phase = static_cast<float>(i) /
                    static_cast<float>(length);
      // Copy in values based on a standard sine wave
      buffer[i] = rt::math::sinusodal(frequency_hz_, phase);
    }
    return buffer;
  }

 private:
  float frequency_hz_;
};

#endif  // APP_PURE_TONE_HPP