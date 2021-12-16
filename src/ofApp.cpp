#include "ofApp.h"

#include "rt_parser.hpp"
#include "rt_hyperparams.hpp"
#include "rt_str.hpp"
#include "rt_rand.hpp"

#include <algorithm>
#include <stdio.h>
#include <set>
#include <ctime>
#include <tuple>

using namespace std;
using namespace rt;

//--------------------------------------------------------------
void ofApp::setup(){
    mAppSetupTime = time(nullptr);
    mEnvironmentChangedTime = mAppSetupTime;
    
    ofBackground(0, 0, 0);  // black
    
    ofSetFrameRate(APP_FRAME_RATE);

    string windowTitle = rt::str::format(
     "Raycaster (depth=%d, spread=%.2f, n=%d)",
     RT_MAX_DEPTH, RT_SPREAD_RADIANS, RT_NUM_RAYS);
    
    ofSetWindowTitle(windowTitle);
    
    // Geometry #1
    mGeometriesList.push_back(Geometry(vector<LineSegment>{
        {{-100, -100}, {-100, +100}},
        {{-200, -100}, {-200, +100}},
//        LineSegment{{-300, -100}, {-300, +100}},
    }));

    /*
-200 +100 -100 +200
   0 +100 +100 +100
+100 +100 +200 -100
+100 -100 -100 -200
-100 -100 -100    0
  25    0   25    50
 -50  +50 -100    0
 +50  -50  +75    0

-200    +200    +200    +200
+200    +200    +200    -200
+200    -200    -200    -200

-400 -400 -400 +400
-400 +400 +400 +400
+400 +400 +400 -400
+400 -400 -400 -400

-300 +300 +300 +300
+300 +300 +300 -300
+300 -300 +300 -300
    */

    // Geometry #2
    mGeometriesList.push_back(Geometry(vector<LineSegment>{
        { {-200, + 100}, { -100 ,+ 200 }},
        { {   0 ,+ 100}, { +100 ,+ 100} },
        { {+100 ,+ 100}, { +200 ,- 100} },
        { {+100 ,- 100}, { -100 ,- 200} },
        { {-100 ,- 100}, { -100 ,   0} },
        { {  25 ,   0}, {   25  ,  50} },
        { { -50 ,+ 50}, { -100  ,  0} },
        { { +50 ,- 50}, {  +75  ,  0} },

        { {-200 ,+ 200}, {    +200, + 200} },
        { {+200 ,+ 200}, {    +200, - 200} },
        { {+200 ,- 200}, {    -200, - 200} },

        { {-400 ,- 400}, { -400, + 400} },
        { {-400 ,+ 400}, { +400, + 400} },
        { {+400 ,+ 400}, { +400, - 400} },
        { {+400 ,- 400}, { -400, - 400} },

        { {-300 ,+ 300}, { +300, + 300} },
        { {+300 ,+ 300}, { +300, - 300} },
        { {+300 ,- 300}, { +300, - 300} },
    }));
    
    Environment env{mGeometriesList[0]};
    mRayTracer = RayTracer{env};
    
    // Audio
    mSoundBuffer.assign(APP_AUDIO_RATE, 0.f);
    
    ofSoundStreamSetup(
           APP_AUDIO_NUM_CHANNELS,
           APP_AUDIO_NUM_INPUTS,
           APP_AUDIO_SAMPLE_RATE,
           APP_AUDIO_BUFFER_SIZE,
           APP_AUDIO_N_NUM_BUFFERS);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    if (ofGetMousePressed()) {
        ofVec2f mousePosition = ofVec2f(ofGetMouseX(), ofGetMouseY());
        mPlayerPointingAt =
            {mousePosition.x - DRAW_WINDOW_WIDTH / 2,
             -1 * (mousePosition.y - DRAW_WINDOW_HEIGHT / 2.f)};
        mEnvironmentChanged = true;
        mEnvironmentChangedTime = time(nullptr);
    }
    
    Ray startingRay({mPlayerLocation.x, mPlayerLocation.y}, {mPlayerPointingAt.x, mPlayerPointingAt.y});
    
    if (mEnvironmentChanged) {
        mCachedPaths = mRayTracer.cast(startingRay,
           RT_SPREAD_RADIANS, RT_NUM_RAYS, RT_MAX_DEPTH);
    }
    
    ofPushMatrix();
    {
        // Center on (0, 0) with positive x-axis to the right and positive
        ofTranslate((DRAW_WINDOW_WIDTH / 2), (DRAW_WINDOW_HEIGHT / 2));
        ofRotateXDeg(180);
        
        // *****
        // DRAW ENVIRONMENT
        // *****
        
        // DRAW GRID LINES
        
        ofSetColor(ofFloatColor(0.5, 0.5, 0.5));  // gray
        
        // Draw rows
        for (int rowY = (DRAW_WINDOW_HEIGHT / 2);
             rowY > (-1 * DRAW_WINDOW_HEIGHT / 2);
             rowY -= DRAW_GRID_SPACING_PX) {
            ofSetLineWidth(1);
            ofDrawLine((-1 * DRAW_WINDOW_WIDTH / 2), (rowY),
                       (+1 * DRAW_WINDOW_WIDTH / 2), (rowY));
            
        }
        
        // Draw columns
        for (int colX = (DRAW_WINDOW_WIDTH / 2);
             colX > (-1 * DRAW_WINDOW_WIDTH / 2);
             colX -= DRAW_GRID_SPACING_PX) {
            ofSetLineWidth(1);
            ofDrawLine((colX), (-1 * DRAW_WINDOW_HEIGHT / 2),
                       (colX), (+1 * DRAW_WINDOW_HEIGHT / 2));
        }
        
        // Draw x-axis
        ofSetLineWidth(2);
        ofSetColor(ofFloatColor(1, 0, 0));
        ofDrawLine((0), (0), (DRAW_GRID_SPACING_PX / 8), (0));
        
        // Draw y-axis
        ofSetLineWidth(2);
        ofSetColor(ofFloatColor(0, 1, 0));
        ofDrawLine((0), (0), (0), (DRAW_GRID_SPACING_PX / 8));
        
        // Draw geometry's edges
        
        rtDraw(mRayTracer.environment);
        
        // Draw ray traces

        rtDraw(mCachedPaths);
        
        vector<Trace<Beam*>*> beams = mRayTracer.beamCast(startingRay, RT_SPREAD_RADIANS, 2);
        
        // Draw beams
        
        rtDraw(beams);
        
        // Draw source ray
        
        rtDraw(startingRay, mPlayerPointingAt);
        
        // Draw metadata
        rtDrawMetadata();
    }
    ofPopMatrix();
    
    mEnvironmentChanged = false;
    
    // Write to the audio buffer
    int nSamplesPerFrame = (APP_AUDIO_RATE / APP_FRAME_RATE);
    for (int i = 0; i < nSamplesPerFrame; i++) {

        float phase = mSoundBufferWriteIndex / (float) APP_AUDIO_RATE;

        assert(0.f <= phase && phase <= 1.f);

        int nTones = 7;
        float mixRatio = 1 / (float) nTones;

        float value = 0;
        value += mixRatio * sinusodal(659.2551, phase); // E4
        value += mixRatio * sinusodal(440.0000, phase); // A4
        value += mixRatio * sinusodal(554.3653, phase); // E4
        value += mixRatio * sinusodal(391.9954, phase); // G3
        value += mixRatio * sinusodal(164.8138, phase); // E3
        value += mixRatio * sinusodal(130.8128, phase); // C3
        value += mixRatio * sinusodal(055.0000, phase); // A1

        float amp = 1 - Vector(startingRay.origin, {0, 0}).magnitude() / 100.f;

        value *= amp;

        mMutex.lock();
        mSoundBuffer[mSoundBufferWriteIndex] = value;
        mSoundBufferWriteIndex = (mSoundBufferWriteIndex + 1) % APP_AUDIO_RATE;
        mMutex.unlock();
    }
    
    
    // Update time variables
    mFrameNum = (mFrameNum + 1) % APP_FRAME_RATE;
    mTotalFrameNum++;
    
    mFpsCounter.newFrame();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_LEFT ||
        key == OF_KEY_RIGHT ||
        key == OF_KEY_UP ||
        key == OF_KEY_DOWN) {
        
        ofVec2f oldSourcePoint = mPlayerLocation;
        const int step = 10;
        if (key == OF_KEY_LEFT) {
            mPlayerLocation.x -= step;
        } else if (key == OF_KEY_RIGHT) {
            mPlayerLocation.x += step;
        } else if (key == OF_KEY_UP) {
            mPlayerLocation.y += step;
        } else if (key == OF_KEY_DOWN) {
            mPlayerLocation.y -= step;
        }
        
        ofVec2f newSourcePoint = mPlayerLocation;
        mPlayerPointingAt += (newSourcePoint - oldSourcePoint);
        
        mEnvironmentChanged = true;
        
    } else if ('1' <= key && key <= '9') {
        int index = key - '1';
        
        if (index < 0 || index > mGeometriesList.size() - 1) {
            return;
        }
        
        Geometry geo = mGeometriesList[index];
        mRayTracer = RayTracer(Environment{geo});
        
        mEnvironmentChanged = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {
    for (int i = 0; i < bufferSize * nChannels; i += nChannels) {
        for (int c = 0; c < nChannels; c++) {
            output[i + c] = mSoundBuffer[mSoundBufferReadIndex];
        }
        mSoundBufferReadIndex = (mSoundBufferReadIndex + nChannels) % APP_AUDIO_RATE;
    }
}

void ofApp::rtDraw(vector<Trace<Vector>*> traces) {
    ofSetColor(ofFloatColor(1, 1, 1, 0.5));  // clear white
    for (Trace<Vector> *trace : mCachedPaths) {
        // Draw out rays with diminishiong power (i.e. lineWidth)
        float lineWidth = 8;
        while (trace != nullptr && lineWidth > 0.) {
            ofSetLineWidth(lineWidth);
            ofDrawLine(trace->data.origin.x, trace->data.origin.y, trace->data.dest.x, trace->data.dest.y);
            // Round caps on ray ends
            ofDrawCircle((trace->data.origin.x), (trace->data.origin.y), 4);
            ofDrawCircle((trace->data.dest.x), (trace->data.dest.y), 4);
            
            // Move forward
            lineWidth *= lineWidth > 0.000001 ? 0.9 : 0.;
            trace = trace->next;
        }
    }
}

void ofApp::rtDraw(Ray startingRay, ofVec2f lookingAt) {
    ofSetColor(ofColor(128, 0, 128));  // purple
    
    ofDrawCircle((startingRay.origin.x), (startingRay.origin.y), 4);
    
    ofSetLineWidth(2);
    ofDrawLine((startingRay.origin.x), (startingRay.origin.y),
               (startingRay(20).x), (startingRay(20).y));
    
    ofDrawCircle((startingRay(20).x), (startingRay(20).y), 2);
    
    ofDrawCircle(lookingAt.x, lookingAt.y, 2);
    
}

void ofApp::rtDraw(vector<Trace<Beam*>*> traces) {
    
    srand(42);
    
    for (Trace<Beam*>* beamTrace : traces) {
        Vector a = beamTrace->data->mBoundA, b = beamTrace->data->mBoundB;
        
        ofSetLineWidth(1);
        ofPath path = ofPath();
        path.moveTo(a.origin.x, a.origin.y);
        path.lineTo(a.dest.x, a.dest.y);
        path.lineTo(b.dest.x, b.dest.y);
        path.lineTo(b.origin.x, b.origin.y);
        ofColor color = ofFloatColor(randF(), randF(), randF(), 0.5); // random
        path.setFillColor(color);
        path.draw();
        
    }
}

void ofApp::rtDraw(Environment env) {
    for (LineSegment edge : env.geometry.edges) {
        ofSetColor(ofFloatColor(1, 0, 0));

        ofSetLineWidth(4);

        // The edge from a to b
        ofDrawLine((edge.a.x), (edge.a.y), (edge.b.x), (edge.b.y));

        // Round caps on wall ends
        ofDrawCircle((edge.a.x), (edge.a.y), 2);
        ofDrawCircle((edge.b.x), (edge.b.y), 2);
    }

}

void ofApp::rtDrawMetadata() {
    float fps = mFpsCounter.fps();
    string metaFormatString = rt::str::format(
      "Frame: %02d/%02d\nFPS:   %.3f\n", mFrameNum, APP_FRAME_RATE, fps);
    
    if (!mEnvironmentChanged) {
        string frameString = rt::str::format(
             "Last draw: %d (sec)", time(nullptr) - mEnvironmentChangedTime);
        metaFormatString += frameString;
    } else {
        metaFormatString += "Calculating...";
    }
    
    ofDrawBitmapString(metaFormatString,
                       DRAW_WINDOW_WIDTH / 2 - 200,
                       - DRAW_WINDOW_HEIGHT / 2 + 50);
}
