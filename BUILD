cc_binary(
    name = "rt",
    srcs = [ "src/main.cpp" ],
    deps = [ "rt_lib" ],
    copts = ["-Isrc/rt"],
)

cc_test(
    name = "rt_tests",
    srcs = glob(["src/rt/*_tests.cpp", "src/rt/*_tests.hpp"]),
    deps = [ "rt_lib", "gtest" ],
    copts = ["-Isrc/rt"], # This is the key part
)

cc_library(
    name = "rt_lib",
    srcs = glob(["src/rt/*.cpp" ], exclude = [ "src/rt/*_tests.cpp" ]),
    hdrs = glob(["src/rt/*.hpp" ], exclude = [ "src/rt/*_tests.hpp" ]),
    strip_include_prefix = "src/rt",
    copts = ["-Isrc/rt"],
)

cc_library(
    name = "gtest",
    srcs = [ "src/ext/gtest/gtest-all.cc" ],
    hdrs = [ "src/ext/gtest/gtest.h" ],
    strip_include_prefix = "src/ext/gtest",
    copts = ["-Isrc/ext/gtest"], # This is the key part
    linkopts = ["-lpthread"],
)