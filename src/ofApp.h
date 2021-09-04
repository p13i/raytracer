#ifndef ofApp_h
#define ofApp_h

#include "ofMain.h"
#include "ofApp_rtDraw.hpp"

#include "rt_raytracer.hpp"

#include <string>
#include <ctime>
#include <deque>
#include <tuple>
#include <map>

using namespace std;
using namespace rt;

// Drawing macros

#define DRAW_WINDOW_WIDTH 800
#define DRAW_WINDOW_HEIGHT DRAW_WINDOW_WIDTH
#define DRAW_GRID_SPACING_PX 50

#define APP_FRAME_RATE 24
#define APP_AUDIO_RATE 44100
#define APP_AUDIO_NUM_CHANNELS 2
#define APP_AUDIO_NUM_INPUTS 0
#define APP_AUDIO_SAMPLE_RATE APP_AUDIO_RATE
#define APP_AUDIO_BUFFER_SIZE 256
#define APP_AUDIO_N_NUM_BUFFERS 4

#include <random>

auto sinusodal = [](int freqencyHz, float phase) {
    return sin(freqencyHz * 2 * PI * phase);
};

auto randF = []() {
    return std::rand() / (float) RAND_MAX;
};


class PureTone {

public:
    PureTone(float frequency): mFrequency(frequency) {}
    
    vector<float> asBuffer(unsigned int length) {
        vector<float> buffer;
        buffer.assign(length, 0.f);
        for (int i = 0; i < length; i++) {
            float phase = (i / (float) length);
            buffer[i] = sinusodal(mFrequency, phase);
        }
        return buffer;
    }
    
    float mFrequency;
};


class rtOfColorManager {
private:
    vector<ofColor> colors;
    unsigned int mCurrentColorIndex = 0;
    
public:
    rtOfColorManager(unsigned int count) {
        for (int i = 0; i < count; i++) {
            colors.push_back(ofColor(randF(), randF(), randF()));
        }
    }
    
    ofColor nextColor() {
        ofColor color = colors[mCurrentColorIndex];
        mCurrentColorIndex = (mCurrentColorIndex + 1) % colors.size();
    }
    
    ofColor reset() {
        mCurrentColorIndex = 0;
    }
};

vector<float> operator * (vector<float>& vec, float mul) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i] *= mul;
    }
    return vec;
}

class FpsCounter {
    
public:
    FpsCounter(unsigned int fps): mFrameRate(fps) {
        
    }
    
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

    ofApp() { }
    
    time_t mAppSetupTime;
    RayTracer mRayTracer;
    ofVec2f mPlayerLocation, mPlayerPointingAt;
    vector<Trace*> mCachedPaths;
    bool mEnvironmentChanged = false;
    time_t mEnvironmentChangedTime;
    unsigned int mFrameNum = 0;
    unsigned int mTotalFrameNum = 0;
    FpsCounter mFpsCounter{APP_FRAME_RATE};
    rtOfColorManager mColorManager{1 << 10};
    ofMutex mMutex;
    vector<Geometry> mGeometriesList;
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioOut( float * output, int bufferSize, int nChannels );
    
    void rtDraw(vector<Trace*> traces);
    void rtDraw(Ray startingRay, ofVec2f lookingAt);
    void rtDraw(vector<Beam*> beams);
    void rtDraw(Environment env);
    void rtDrawMetadata();
    
private:
    vector<float> mSoundBuffer;
    unsigned int mSoundBufferReadIndex = 0;
    unsigned int mSoundBufferWriteIndex = 0;
};

#endif // ofApp_h
