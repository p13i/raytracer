#pragma once

#include "ofMain.h"

#include "raytracer.hpp"

// Drawing macros

#define DRAW_WINDOW_WIDTH 400
#define DRAW_WINDOW_HEIGHT DRAW_WINDOW_WIDTH
#define DRAW_GRID_SPACING_PX 100
#define DRAWX(x) (x)
#define DRAWY(y) (-1 * (y))
#define DRAW_POINT_FOR_ARGS(p) DRAWX(p.x), DRAWY(p.y)

class ofApp : public ofBaseApp{

	public:
    
        ofApp() { }
    
        rt::RayTracer raytracer;
        ofVec2f sourcePoint, pointingAt;
        std::vector<rt::Vector> cachedPaths;
    
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
