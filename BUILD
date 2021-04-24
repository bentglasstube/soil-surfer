package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "ld48",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "game_screen.cc",
        "title_screen.cc",
    ],
    hdrs = [
        "game_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:screen",
        "@libgam//:text",
        "map",
    ],
)

cc_library(
    name = "map",
    srcs = [ "map.cc" ],
    hdrs = [
        "map.h",
        "stb_perlin.h"
    ],
    deps = [
        "@libgam//:spritemap",
        "@libgam//:util",
    ],
)
