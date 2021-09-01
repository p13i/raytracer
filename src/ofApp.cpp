#include "ofApp.h"

#include "rt_parser.hpp"
#include "rt_hyperparams.hpp"
#include "rt_str.hpp"

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
    
    Geometry geo = rt::parser::readGeometry(RT_ENVIRONMENT_FILE);
    Environment env{geo};
    mRayTracer = RayTracer{env};
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
        
        for (LineSegment edge : mRayTracer.environment.geometry.edges) {
            ofSetColor(ofFloatColor(1, 0, 0));

            ofSetLineWidth(4);

            // The edge from a to b
            ofDrawLine((edge.a.x), (edge.a.y), (edge.b.x), (edge.b.y));

            // Round caps on wall ends
            ofDrawCircle((edge.a.x), (edge.a.y), 2);
            ofDrawCircle((edge.b.x), (edge.b.y), 2);
        }

        // Draw ray traces

        ofSetColor(ofFloatColor(1, 1, 1, 0.5));  // clear white

        for (Trace *trace : mCachedPaths) {
            // Draw out rays with diminishiong power (i.e. lineWidth)
            float lineWidth = 8;
            while (trace != nullptr && lineWidth > 0.) {
                ofSetLineWidth(lineWidth);
                ofDrawLine(trace->vec.origin.x, trace->vec.origin.y, trace->vec.dest.x, trace->vec.dest.y);
                // Round caps on ray ends
                ofDrawCircle((trace->vec.origin.x), (trace->vec.origin.y), 4);
                ofDrawCircle((trace->vec.dest.x), (trace->vec.dest.y), 4);
                
                // Move forward
                lineWidth *= lineWidth > 0.000001 ? 0.9 : 0.;
                trace = trace->next;
            }

        }
        
        // Draw source ray
        
        ofSetColor(ofColor(128, 0, 128));  // purple
        
        ofDrawCircle((startingRay.origin.x), (startingRay.origin.y), 4);
        
        ofSetLineWidth(2);
        ofDrawLine((startingRay.origin.x), (startingRay.origin.y),
                   (startingRay(20).x), (startingRay(20).y));
        
        ofDrawCircle((startingRay(20).x), (startingRay(20).y), 2);
        
        ofDrawCircle(mPlayerPointingAt.x, mPlayerPointingAt.y, 2);
        
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
    ofPopMatrix();
    
    mEnvironmentChanged = false;
    
    // Update time variables
    mFrameNum = (mFrameNum + 1) % APP_FRAME_RATE;
    mTotalFrameNum++;
    
    
    mFpsCounter.newFrame();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    const static std::set<int> handledKeys = {
        OF_KEY_LEFT, OF_KEY_RIGHT, OF_KEY_UP, OF_KEY_DOWN
    };
    
    if (handledKeys.find(key) == handledKeys.end()) {
        return;
    }
    
    ofVec2f oldSourcePoint = mPlayerLocation;
    
    const int step = 2;
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
