#include "ofApp.h"

#include <algorithm>
#include <stdio.h>
#include <set>

#include "rt_parser.hpp"
#include "rt_hyperparams.hpp"
#include "rt_str.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);  // black
    
    ofSetFrameRate(24);

#ifdef DRAW_TRACE
    auto tracerType = "Raytracer";
#endif
#ifdef DRAW_CAST
    auto tracerType = "Raycaster";
#endif
    
    ofSetWindowTitle(rt::str::format("%s (depth=%d, spread=%.2f, n=%d)",
                                     tracerType, RT_MAX_DEPTH, RT_SPREAD_RADIANS, RT_NUM_RAYS));
    
    rt::Geometry geo = rt::parser::readGeometry(RT_ENVIRONMENT_FILE);
    rt::Environment env{geo};
    this->raytracer = rt::RayTracer{env};
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    if (ofGetMousePressed()) {
        ofVec2f mousePosition = ofVec2f(ofGetMouseX(), ofGetMouseY());
        pointingAt = {mousePosition.x - DRAW_WINDOW_WIDTH / 2, -1 * (mousePosition.y - DRAW_WINDOW_HEIGHT / 2.f)};
        environmentChanged = true;
    }
    
    rt::Ray startingRay({sourcePoint.x, sourcePoint.y}, {pointingAt.x, pointingAt.y});
    
    if (environmentChanged) {
        
#ifdef DRAW_TRACE
        // Trace rays
        cachedPaths = raytracer.trace(startingRay, RT_MAX_DEPTH);
#endif
        
#ifdef DRAW_CAST
        cachedPaths = raytracer.cast(startingRay, RT_SPREAD_RADIANS, RT_NUM_RAYS, RT_MAX_DEPTH);
#endif
        
        environmentChanged = false;
    }
    
    ofPushMatrix();
    {
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
        
        std::vector<rt::LineSegment> edges;
        
        for (auto edge : this->raytracer.environment.geometry.edges) {
            ofSetColor(ofFloatColor(1, 0, 0));

            float lineWidth = 4;
            ofSetLineWidth(lineWidth);

            // The edge from a to b
            ofDrawLine((edge.a.x), (edge.a.y),
                       (edge.b.x), (edge.b.y));

            // Round caps on wall ends
            ofDrawCircle((edge.a.x), (edge.a.y), lineWidth / 2);
            ofDrawCircle((edge.b.x), (edge.b.y), lineWidth / 2);
        }

        // Draw ray traces

        ofSetColor(ofFloatColor(1, 1, 1, 0.5));  // clear white

#ifdef DRAW_CAST
        for (rt::Trace *trace : cachedPaths) {
#endif

#ifdef DRAW_TRACE
            auto paths = cachedPaths;
#endif
            int lineWidth = 8;
            while (trace != nullptr && lineWidth > 0.) {
                ofSetLineWidth(lineWidth);
                ofDrawLine(trace->vec.origin.x, trace->vec.origin.y, trace->vec.dest.x, trace->vec.dest.y);
                // Round caps on ray ends
                ofDrawCircle((trace->vec.origin.x), (trace->vec.origin.y), 4);
                ofDrawCircle((trace->vec.dest.x), (trace->vec.dest.y), 4);
                lineWidth *= lineWidth > 0.000001 ? 0.9 : 0.;
                trace = trace->next;
            }

#ifdef DRAW_CAST
        }
#endif
        
        // Draw source ray
        
        ofSetColor(ofColor(128, 0, 128));  // purple
        
        ofDrawCircle((startingRay.origin.x), (startingRay.origin.y), 4);
        
        ofSetLineWidth(2);
        ofDrawLine((startingRay.origin.x), (startingRay.origin.y),
                   (startingRay(20).x), (startingRay(20).y));
        
        ofDrawCircle((startingRay(20).x), (startingRay(20).y), 2);
        
        ofDrawCircle(pointingAt.x, pointingAt.y, 2);
        
    }
    ofPopMatrix();
    
//    mFrameNum++;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    const static std::set<int> handledKeys = {
        OF_KEY_LEFT, OF_KEY_RIGHT, OF_KEY_UP, OF_KEY_DOWN
    };
    
    if (handledKeys.find(key) == handledKeys.end()) {
        return;
    }
    
    ofVec2f oldSourcePoint = sourcePoint;
    
    const int step = 2;
    if (key == OF_KEY_LEFT) {
        sourcePoint.x -= step;
    } else if (key == OF_KEY_RIGHT) {
        sourcePoint.x += step;
    } else if (key == OF_KEY_UP) {
        sourcePoint.y += step;
    } else if (key == OF_KEY_DOWN) {
        sourcePoint.y -= step;
    }
    
    ofVec2f newSourcePoint = sourcePoint;
    
    pointingAt += (newSourcePoint - oldSourcePoint);
    
    environmentChanged = true;
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
