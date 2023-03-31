#include "rt_tests.hpp"

#include <cstdio>

#include "gtest.h"

int rt::test::main(int argc, char **argv) {
  printf("Running main() from rt_tests.cpp\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}