#include "app_settings.hpp"
#include "rt_tests.hpp"

#if APP_ENABLE
#include "ofApp.h"
#endif  // APP_ENABLE

int main(int argc, char *argv[]) {
  // Run tests first
  const int test_result = rt::test::main(argc, argv);

#if APP_ENABLE
  // Open Frameworks app:
  ofSetupOpenGL(APP_DRAW_WINDOW_WIDTH,
                APP_DRAW_WINDOW_HEIGHT, OF_WINDOW);

  // this kicks off the running of my app
  // can be OF_WINDOW or OF_FULLSCREEN
  // pass in width and height too:
  ofRunApp(new ofApp());
#endif  // APP_ENABLE

  return test_result;
}