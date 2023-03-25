#include "ofApp.h"
#include "ofMain.h"
#include "rt_raytracer.hpp"
#include "rt_tests.hpp"

//========================================================================
int main(int argc, char *argv[]) {
  // Run tests first
  const int test_result = test::main(argc, argv);
  if (test_result != 0) {
    return test_result;
  }

#define APP_ENABLE false
#if APP_ENABLE
  // Open Frameworks app:
  ofSetupOpenGL(DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, OF_WINDOW);

  // this kicks off the running of my app
  // can be OF_WINDOW or OF_FULLSCREEN
  // pass in width and height too:
  ofRunApp(new ofApp());
#endif  // APP_ENABLE
}