# Pong

This is a small game of Pong created in C++ with the intent of mimicking genuine
interactions with the game environment. Because of this goal, the players of the
game are run on different threads than the game itself. I recently watched a
couple of
[Reinforcement Learning lectures by David Silver](http://www0.cs.ucl.ac.uk/staff/d.silver/web/Teaching.html)
and wanted apply my knowledge by implementing some of the methods described in
the lectures.

## Prerequisites
  * [`clang`](https://clang.llvm.org/) `3.4`+ (`C++ 14`)
  * [`bazel`](https://bazel.build/)
  * [`pygame`](https://www.pygame.org/)
  * [`matplotlib`](https://matplotlib.org/)

## Building and running

From the `src/` directory, run:
```
CC=clang bazel build -c opt //main:main
```
to build an optimized build of the program.

Or, it is possible to build and run in one step:
```
CC=clang bazel run -c opt //main:main
```

## Visualization
There is a small `Python` visualizer in `src/tools/visualize.py` which reads text
from `stdin` and draws the corresponding state of the pong game. In the future,
this is going to be replaced by a native `C++` framework. For now, here is one
example of running the visualizer to update at `60Hz`:
```
CC=clang bazel run //main:main | ./tools/visualize.py \
    --speed 60 \
    --width 1000 \
    --height 1000
```

## Contact
Email: [davidb2@illinois.edu](mailto:davidb2@illinois.edu)
