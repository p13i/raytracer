#include "ofMain.h"
#include "ofApp.h"

#include "rt_raytracer.hpp"
#include "rt_tests.hpp"

//========================================================================
int main(int argc, char *argv[]){
    
    if (argc >= 2 && std::string(argv[1]) == "run-tests") {
        rt::test::run_tests();
        return;
    }
    
    ofSetupOpenGL(DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    ofRunApp(new ofApp());
}
