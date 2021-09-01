#pragma once

#include "ofMain.h"

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

#define APP_FRAME_RATE 60

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

class ofApp : public ofBaseApp{

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
};
