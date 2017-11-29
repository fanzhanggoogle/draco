#Description:
#Basic data structures for storing point cloud data.

draco_root = "third_party/draco/src/"
package(default_copts = ["-I" + draco_root],
        default_hdrs_check = "strict",
        default_visibility = ["//visibility:public"],
)

licenses(["notice"]) #Apache 2.0

exports_files(["LICENSE"])

cc_library(
    name = "keyframe_animation",
    srcs = [
        "keyframe_animation.cc",
    ],
    hdrs = [
        "keyframe_animation.h",
        
    ],
    deps = [
        "//third_party/draco/src/draco/point_cloud:point_cloud",
    ],
)

cc_test(
    name = "keyframe_animation_test",
    size = "small",
    timeout = "short",
    srcs = ["keyframe_animation_test.cc"],
    deps = [
        ":keyframe_animation",
        "//testing/base/public:gunit_main",
        "//third_party/draco/src/draco/core:test_utils",
    ],
)

cc_library(
    name = "keyframe_animation_encoder",
    srcs = [
        "keyframe_animation_encoder.cc",
    ],
    hdrs = [
        "keyframe_animation_encoder.h",
        
    ],
    deps = [
        ":keyframe_animation",
        "//third_party/draco/src/draco/compression/point_cloud:point_cloud_sequential_encoder",
    ],
)

cc_library(
    name = "keyframe_animation_decoder",
    srcs = [
        "keyframe_animation_decoder.cc",
    ],
    hdrs = [
        "keyframe_animation_decoder.h",
        
    ],
    deps = [
        ":keyframe_animation",
        "//third_party/draco/src/draco/compression/point_cloud:point_cloud_sequential_decoder",
    ],
)

cc_test(
    name = "keyframe_animation_encoding_test",
    size = "small",
    timeout = "short",
    srcs = ["keyframe_animation_encoding_test.cc"],
    deps = [
        ":keyframe_animation",
        ":keyframe_animation_encoder",
        ":keyframe_animation_decoder",
        "//testing/base/public:gunit_main",
        "//third_party/draco/src/draco/core:test_utils",
    ],
)
