//
//  rt_linalg_tests.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 2/5/22.
//

#include "gtest.h"
#include "rt_linalg.hpp"

using namespace rt::linalg;

TEST(Matrix2x2, determinant) {
  {
    Matrix2x2 m{1, 2, 3, 4};
    EXPECT_EQ(-2, m.determinant());
  }

  {
    Matrix2x2 m{2, 3, 4, 1};
    EXPECT_EQ(-10, m.determinant());
  }

  {
    Matrix2x2 m{3, 4, 1, 2};
    EXPECT_EQ(2, m.determinant());
  }
}