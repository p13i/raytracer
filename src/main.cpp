#include "ofMain.h"
#include "ofApp.h"

#include "raytracer.hpp"

//========================================================================
int main( ){
//    rt::main();
    
    ofSetupOpenGL(DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
    
    PRINTLN("Starting ofApp");
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    ofRunApp(new ofApp());
    
    
    PRINTLN("App exiting.");
}
