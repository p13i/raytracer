#ifndef OF_APP_H
#define OF_APP_H

#include <ctime>
#include <vector>

#include "app_color_manager.hpp"
#include "app_fps_counter.hpp"
#include "app_settings.hpp"
#include "ofMain.h"
#include "rt_raytracer.hpp"

// Main Open Frameworks app
class ofApp final : public ofBaseApp {
 public:
  ofApp() = default;
  // Called before the app starts
  void setup() override;
  // Called right before draw(), meant to do non-app
  // rendering/graphics such as running the beam casting
  // algorithm or writing to the audio buffer.
  void update() override;
  // Called at the specified frame rate after update() and
  // should be used solely for drawing scene information to
  // the OpenFrameworks GUI
  void draw() override;
  // Handles the key-down event
  void keyPressed(int key) override;

#if APP_ENABLE_AUDIO
  // Writes to the audio buffer
  void audioOut(ofSoundBuffer &soundBuffer) override;
#endif  // APP_ENABLE_AUDIO

  // Draws the player's position and orientation
  void rtDraw(rt::Ray startingRay, ofVec2f lookingAt);
  // Draws traces to the environment consisting of vectors,
  // i.e. a standard ray-tracer "ray"
  void rtDrawTraceOfVectors(
      std::vector<rt::Trace<rt::Vector> *> traces);
  // Draws traces with beams, i.e. "beam casting" rays
  void rtDrawTracesOfBeams(
      vector<rt::Trace<rt::Beam *> *> traces);
  // Draws the environment
  void rtDraw(rt::Environment env) const;
  // Draws the app metadata in the corner
  void rtDrawMetadata() const;

 private:
  // Core instance
  rt::RayTracer raytracer_;
  // Available scene geometries, indexed by the number keys
  vector<rt::Geometry> geometries_list_;
  // When the app setup() function was first called
  time_t app_setup_time_{};
  // Player information
  ofVec2f player_location_, player_pointing_at_;
  // Caching RayTracer results to save computation for
  // un-changed scenes.
  vector<rt::Trace<rt::Vector> *> cached_paths_;
  // Tracks if the player has moved or the environment has
  // changed.
  bool environment_changed_ = false;
  // Tracks when the environment was changed.
  time_t environment_changed_time_{};
  // The current frame number from 0 to the FPS.
  unsigned int frame_num_ = 0;
  // The current frame number from the start of the app.
  unsigned int total_frame_num_ = 0;
  // Computes the frame rate.
  AppFpsCounter fps_counter_{APP_FRAME_RATE};
  // Computes and delegates the colors used while drawing.
  AppColorManager color_manager_{1 << 10};

#if APP_ENABLE_AUDIO
  // Used to control when sound buffer-related writes are
  // happening to avoid dead locks.
  ofMutex sound_buffer_mutex_;
  vector<float> mSoundBuffer;
  unsigned int mSoundBufferReadIndex = 0;
  unsigned int mSoundBufferWriteIndex = 0;
#endif  // APP_ENABLE_AUDIO
};

#endif  // OF_APP_H