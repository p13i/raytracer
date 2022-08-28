cc_test(
    name = "rt_tests",
    srcs = [ "src/main.cpp" ],
    deps = [ "rt" ],
)

cc_library(
    name = "rt",
    srcs = glob(["src/rt/*.cpp"]),
    hdrs = glob(["src/rt/*.hpp"]),
    deps = [ "gtest" ]
)

cc_library(
    name = "gtest",
    srcs = [ "src/ext/gtest/gtest-all.cc" ],
    hdrs = [ "src/ext/gtest/gtest.h" ],
    linkopts = ["-lpthread"],
)