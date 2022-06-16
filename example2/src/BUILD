load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "example2",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",
    ],
)