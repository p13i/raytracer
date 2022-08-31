//
//  rt_linalg_tests.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 2/5/22.
//

#include "rt_linalg.hpp"

#include "gtest.h"

using namespace rt::linalg;

TEST(Matrix2x2, determinant) {
    EXPECT_EQ(-2, Matrix2x2(1, 2, 3, 4).determinant());
    EXPECT_EQ(-10, Matrix2x2(2, 3, 4, 1).determinant());
    EXPECT_EQ(2, Matrix2x2(3, 4, 1, 2).determinant());
}
