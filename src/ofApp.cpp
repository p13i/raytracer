#include "ofApp.h"

#include <algorithm>
#include <stdio.h>
#include <set>

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);  // black
    
    ofSetFrameRate(24);
    
    
    rt::Environment env{
        /* geometry: */ {
            /* edges: */ std::vector<rt::LineSegment>({
                rt::LineSegment({-200, +100}, {-100, +200}),
                rt::LineSegment({   0, +100}, {+100, +100}),
                rt::LineSegment({+100, +100}, {+200, -100}),
                rt::LineSegment({+100, -100}, {-100, -200}),
                rt::LineSegment({-100, -100}, {-100,    0}),
                rt::LineSegment({  25,    0}, {25,    50}),
                rt::LineSegment({ -50,  +50}, {-100,    0}),
                rt::LineSegment({ +50,  -50}, { +75,    0}),
            })
        }
    };
    
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
    
    auto startingRay = rt::Ray({sourcePoint.x, sourcePoint.y}, {pointingAt.x, pointingAt.y});
    
    if (environmentChanged) {
        
#ifdef DRAW_TRACE
        // Trace rays
        cachedPaths = raytracer.trace(startingRay, RT_MAX_DEPTH);
#endif
        
#ifdef DRAW_CAST
        cachedPaths = raytracer.cast(startingRay, M_PI / 8.f, 9, RT_MAX_DEPTH);
#endif
        
        environmentChanged = false;
    }
    
    ofPushMatrix();
    {
        ofTranslate((DRAW_WINDOW_WIDTH / 2), (DRAW_WINDOW_HEIGHT / 2));
        
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
            ofDrawLine(DRAWX(-1 * DRAW_WINDOW_WIDTH / 2), DRAWY(rowY),
                       DRAWX(+1 * DRAW_WINDOW_WIDTH / 2), DRAWY(rowY));
        }
        
        // Draw columns
        for (int colX = (DRAW_WINDOW_WIDTH / 2);
             colX > (-1 * DRAW_WINDOW_WIDTH / 2);
             colX -= DRAW_GRID_SPACING_PX) {
            ofSetLineWidth(1);
            ofDrawLine(DRAWX(colX), DRAWY(-1 * DRAW_WINDOW_HEIGHT / 2),
                       DRAWX(colX), DRAWY(+1 * DRAW_WINDOW_HEIGHT / 2));
        }
        
        // Draw x-axis
        ofSetLineWidth(2);
        ofSetColor(ofFloatColor(1, 0, 0));
        ofDrawLine(DRAWX(0), DRAWY(0), DRAWX(DRAW_GRID_SPACING_PX / 8), DRAWY(0));
        
        // Draw y-axis
        ofSetLineWidth(2);
        ofSetColor(ofFloatColor(0, 1, 0));
        ofDrawLine(DRAWX(0), DRAWY(0), DRAWX(0), DRAWY(DRAW_GRID_SPACING_PX / 8));
        
        // Draw geometry's edges
        
        std::vector<rt::LineSegment> edges;
        
        for (auto edge : this->raytracer.environment.geometry.edges) {
            ofSetColor(ofFloatColor(1, 0, 0));

            float lineWidth = 4;
            ofSetLineWidth(lineWidth);

            // The edge from a to b
            ofDrawLine(DRAWX(edge.a.x), DRAWY(edge.a.y),
                       DRAWX(edge.b.x), DRAWY(edge.b.y));

            // Round caps on wall ends
            ofDrawCircle(DRAWX(edge.a.x), DRAWY(edge.a.y), lineWidth / 2);
            ofDrawCircle(DRAWX(edge.b.x), DRAWY(edge.b.y), lineWidth / 2);
        }

        // Draw ray traces

        ofSetColor(ofFloatColor(1, 1, 1, 0.5));  // clear white

#ifdef DRAW_CAST
        for (auto cast : cachedPaths) {
            auto paths = cast;
#endif
#ifdef DRAW_TRACE
            auto paths = cachedPaths;
#endif
        int lineWidth = RT_MAX_DEPTH;
        for (auto trace : paths) {
            ofSetLineWidth(lineWidth);
            ofDrawLine(DRAW_POINT_FOR_ARGS(trace.origin), DRAW_POINT_FOR_ARGS(trace.dest));
            // Round caps on ray ends
            ofDrawCircle(DRAWX(trace.origin.x), DRAWY(trace.origin.y), 4);
            ofDrawCircle(DRAWX(trace.dest.x), DRAWY(trace.dest.y), 4);
            lineWidth--;
        }
            
#ifdef DRAW_CAST
        }
#endif
        
        // Draw source ray
        
        ofSetColor(ofColor(128, 0, 128));  // purple
        
        ofDrawCircle(DRAWX(startingRay.origin.x), DRAWY(startingRay.origin.y), 4);
        
        ofSetLineWidth(2);
        ofDrawLine(DRAWX(startingRay.origin.x), DRAWY(startingRay.origin.y),
                   DRAWX(startingRay(20).x), DRAWY(startingRay(20).y));
        
        ofDrawCircle(DRAWX(startingRay(20).x), DRAWY(startingRay(20).y), 2);
        
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
    
    const int step = 10;
    if (key == OF_KEY_LEFT) {
        sourcePoint.x -= step;
    } else if (key == OF_KEY_RIGHT) {
        sourcePoint.x += step;
    } else if (key == OF_KEY_UP) {
        sourcePoint.y += step;
    } else if (key == OF_KEY_DOWN) {
        sourcePoint.y -= step;
    }
    
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
