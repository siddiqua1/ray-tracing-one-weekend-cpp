# ray-tracing-one-weekend-cpp

## Requirements

For compiling this project from source you will need the following

- CMake 3.15 or later
- C++20 Compiler

## Building

```shell
cmake -B build
cmake --build build
```

## Cleaning

```shell
cmake --build build --target clean
```

## Applications

The project builds multiple binaries corresponding to the various checkpoints in the series.

![Output of final render with ray depth of 10](resources/final_scene.png)

### Roadmap

- [x] Get first iteration from the book working
- [x] Update codebase to use more modern C++ and CMake features
- [ ] Benchmarking framework
- [ ] Render modes
  - Toggle between file out put and live render
- [ ] Wasm build for live in browser render
- [ ] Optimize codebase (ECS?)

### References

[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
