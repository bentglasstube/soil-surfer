package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "soil-surfer",
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
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:text",
        ":camera",
        ":centipede",
        ":map",
        ":player",
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
        "@libgam//:audio",
        "@libgam//:spritemap",
        "@libgam//:util",
        ":geometry",
    ],
)

cc_library(
    name = "player",
    srcs = [ "player.cc" ],
    hdrs = [ "player.h" ],
    deps = [
        "@libgam//:audio",
        "@libgam//:spritemap",
        ":centipede",
        ":map",
    ],
)

cc_library(
    name = "camera",
    srcs = [ "camera.cc" ],
    hdrs = [ "camera.h" ],
    deps = [
        "@libgam//:rect",
        ":player",
    ]
)

cc_library(
    name = "geometry",
    srcs = ["geometry.cc"],
    hdrs = ["geometry.h"],
)

cc_library(
    name = "centipede",
    srcs = ["centipede.cc"],
    hdrs = ["centipede.h"],
    deps = [
        "@libgam//:audio",
        "@libgam//:spritemap",
        ":geometry",
        ":map",
    ]
)
