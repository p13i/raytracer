//
//  rt_tests.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 2/3/22.
//

#include "rt_tests.hpp"

#include <stdio.h>

#include "gtest.h"

int rt::test::main(int argc, char **argv) {
  printf("Running main() from rt_tests.cpp\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}