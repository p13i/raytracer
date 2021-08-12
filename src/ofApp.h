#pragma once

#include "ofMain.h"

#include "rt_raytracer.hpp"

#include <string>

// Drawing macros

#define DRAW_WINDOW_WIDTH 800
#define DRAW_WINDOW_HEIGHT DRAW_WINDOW_WIDTH
#define DRAW_GRID_SPACING_PX 50

class ofApp : public ofBaseApp{

	public:
    
        ofApp() { }
    
        rt::RayTracer raytracer;
    
        ofVec2f sourcePoint, pointingAt;
    
#ifdef DRAW_TRACE
        std::vector<rt::Vector> cachedPaths;
#endif

#ifdef DRAW_CAST
        std::vector<rt::Trace*> cachedPaths;
#endif
    
        bool environmentChanged = false;
    
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
