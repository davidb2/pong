cc_library(
    name = "pong",
    visibility = ["//visibility:public"],
    srcs = [
        "Action.C",
        "Environment.C",
        "Game.C",
        "Manager.C",
        "Reward.C",
        "State.C",
    ],
    hdrs = [
        "Action.H",
        "Agent.H",
        "Environment.H",
        "Game.H",
        "Manager.H",
        "Reward.H",
        "State.H",
    ],
    copts = [
        "-std=c++14",
        "-Wfatal-errors",
        "-Wall",
        "-pedantic",
    ],
    linkopts = [
        "-pthread",
    ],
)
