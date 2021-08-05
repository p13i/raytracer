//
//  rt_tests.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_tests_hpp
#define rt_tests_hpp

#include "rt_str.hpp"
#include "rt_log.hpp"

#define RT_TEST(case, code) rt::log(rt::str::format("➡️ %s", case)); code;
#define RT_ASSERT(cond, msg) if (!(cond)) { rt::log("❌ FAIL ", true, false); rt::log(msg, false, true); exit(1); } else { rt::log("✅ PASS ", true, false); rt::log(msg, false, true); }


namespace rt { namespace test {

void run_tests();

void Fail(const char *msg);
void Assert(bool cond, const char *msg);
void AssertEqual(float a, float b, const char *msg);

void Test(const char *msg, std::function<void()> lambda);

} }

#endif /* rt_tests_hpp */
