#include "ofMain.h"
#include "ofApp.h"

#define PRINTLN(x) std::cout << x << std::endl;


#include "rt_raytracer.hpp"
#include "rt_tests.hpp"

//========================================================================
int main( ){
    rt::test::run_tests();
    
    ofSetupOpenGL(DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
    
    PRINTLN("Starting ofApp");
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    ofRunApp(new ofApp());
    
    
    PRINTLN("App exiting.");
}
