#ifndef ofApp_h
#define ofApp_h

#include <ctime>
#include <deque>
#include <map>
#include <string>
#include <tuple>

#include "app_settings.hpp"
#include "ofMain.h"
#include "rt_random.hpp"
#include "rt_raytracer.hpp"

using namespace std;
using namespace rt;

// Drawing macros

#define DRAW_WINDOW_WIDTH 800
#define DRAW_WINDOW_HEIGHT DRAW_WINDOW_WIDTH
#define DRAW_GRID_SPACING_PX 50

#include <random>

#include "rt_math.hpp"

class PureTone {
 public:
  PureTone(float frequency) : mFrequency(frequency) {}

  vector<float> asBuffer(size_t length) {
    vector<float> buffer;
    buffer.assign(length, 0.f);
    for (size_t i = 0; i < length; i++) {
      float phase = (i / static_cast<float>(length));
      buffer[i] = math::sinusodal(mFrequency, phase);
    }
    return buffer;
  }

  float mFrequency;
};

class rtOfColorManager {
 private:
  vector<ofColor> colors;
  size_t mCurrentColorIndex = 0;

 public:
  rtOfColorManager(unsigned int count) {
    for (size_t i = 0; i < count; i++) {
      colors.push_back(
          ofColor(random::randF(), random::randF(), random::randF()));
    }
  }

  ofColor nextColor() {
    ofColor color = colors[mCurrentColorIndex];
    mCurrentColorIndex = (mCurrentColorIndex + 1) % colors.size();
    return color;
  }

  void reset() { mCurrentColorIndex = 0; }
};

#if 0
vector<float> operator * (vector<float>& vec, float mul) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i] *= mul;
    }
    return vec;
}
#endif

class FpsCounter {
 public:
  FpsCounter(unsigned int fps) : mFrameRate(fps) {}

  void newFrame() {
    while (mTimestampsDeque.size() > (mFrameRate * 5)) {
      mTimestampsDeque.pop_front();
    }

    mTimestampsDeque.push_back(time(nullptr));
  }

  float fps() {
    if (mTimestampsDeque.size() < 2) {
      return -1;
    }
    long int oldestFrameTimestampSec = mTimestampsDeque.front();
    long int newestFrameTimestampSec = mTimestampsDeque.back();

    float deltaInSec = newestFrameTimestampSec - oldestFrameTimestampSec;
    return mTimestampsDeque.size() / deltaInSec;
  }

 private:
  unsigned int mFrameRate = 0;
  deque<long int> mTimestampsDeque;
};

class ofApp : public ofBaseApp {
 public:
  ofApp() {}

  time_t mAppSetupTime;
  RayTracer mRayTracer;
  ofVec2f mPlayerLocation, mPlayerPointingAt;
  vector<Trace<Vector>*> mCachedPaths;
  bool mEnvironmentChanged = false;
  time_t mEnvironmentChangedTime;
  unsigned int mFrameNum = 0;
  unsigned int mTotalFrameNum = 0;
  FpsCounter mFpsCounter{APP_FRAME_RATE};
  rtOfColorManager mColorManager{1 << 10};
  ofMutex mMutex;
  vector<Geometry> mGeometriesList;

  void setup() override;
  void update() override;
  void draw() override;
  void keyPressed(int key) override;
  void keyReleased(int key) override;
  void mouseMoved(int x, int y) override;
  void mouseDragged(int x, int y, int button) override;
  void mousePressed(int x, int y, int button) override;
  void mouseReleased(int x, int y, int button) override;
  void mouseEntered(int x, int y) override;
  void mouseExited(int x, int y) override;
  void windowResized(int w, int h) override;
  void dragEvent(ofDragInfo dragInfo) override;
  void gotMessage(ofMessage msg) override;

#if APP_ENABLE_AUDIO
  void audioOut(ofSoundBuffer& soundBuffer);
#endif  // APP_ENABLE_AUDIO

  void rtDraw(vector<Trace<Vector>*> traces);
  void rtDraw(Ray startingRay, ofVec2f lookingAt);
  void rtDraw(vector<Trace<Beam*>*> beams);
  void rtDraw(Environment env);
  void rtDrawMetadata();

#if APP_ENABLE_AUDIO
  vector<float> mSoundBuffer;
  unsigned int mSoundBufferReadIndex = 0;
  unsigned int mSoundBufferWriteIndex = 0;
#endif  // APP_ENABLE_AUDIO
};

#endif  // ofApp_h