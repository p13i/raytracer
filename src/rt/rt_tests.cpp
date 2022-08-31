//
//  rt_tests.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 2/3/22.
//

#include <stdio.h>

#include "gtest.h"

int main(int argc, char **argv) {
  printf("Running main() from rt_tests.cpp\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
