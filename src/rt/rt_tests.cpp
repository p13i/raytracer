//
//  rt_tests.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_tests.hpp"

#include "rt_point.hpp"
#include "rt_geo.hpp"
#include "rt_log.hpp"
#include "rt_str.hpp"
#include "rt_raytracer.hpp"
#include "rt_ray.hpp"


void rt::test::run_tests() {
    
}

void rt::test::Fail(const char *msg) {
    LOG("Assertion failed.");
    LOG(msg);
    exit(1);
}

void rt::test::Assert(bool cond, const char *msg) {
    if (!cond) {
        return Fail(msg);
    }
}

void rt::test::AssertEqual(float a, float b, const char *msg) {
    if (abs(a - b) > 0.001) {
        auto m = rt::str::format("%f != %f", a, b).c_str();
        return Fail(m);
    }
}

void rt::test::Test(const char *msg, std::function<void()> lambda) {
    rt::log(rt::str::format("➡️ %s", msg));
    lambda();
}
