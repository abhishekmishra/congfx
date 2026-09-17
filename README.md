# ConGfx
ConGfx is an experimental library for having fun with terminal programs written in C. It is heavily inspired by Love2D and Raylib.

## Build

Build the examples out of source with CMake:

```sh
cmake -S . -B build
cmake --build build
```

The executables are written to `build/examples/`.

Generate the API and site documentation with:

```sh
cmake --build build --target docs
```
