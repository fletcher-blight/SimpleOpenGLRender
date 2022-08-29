# Simple OpenGL Renderer

Scratch project for learning to render with openGL

Standard CMake build process:
```
cmake [options...] -S . -B build
cmake --build build --target renderer
```

With the executable found at `./build/src/renderer`

For reproducible builds, use docker images from `fletcherblight/buildenv-cpp-<platform>`.
So for creating a windows binary:
```
docker run --rm --it -v ${PWD}:/src -w /src fletcherblight/buildenv-cpp-windows
[/src]# <follow build commands from above>
# Run natively on windows
```