# NinthEngine
3D Render Engine written in C++

Open source and distributed under MIT License.

# Features
The engine is split into (mostly) independent modules, aside from the core module.

Platforms -
* Windows (Win32)
* [Planned] Linux (X11)

Graphics API -
* OpenGL 4.0
* Direct3D 11.0
* [Planned] Vulkan

# Build
1. Clone the repository.
2. Update submodules
    ```shell
    git submodule update --init
    ```
3. Open NinthEngineCore in Microsoft Visual Studio (core/build/) and build.
4. Build all the modules you want to use in the same way as the core.
5. Create a new project and link the NinthEngineCore.lib and core/include/ along with any modules you want to use.

# Getting Started
WIP

# Credit
* OpenGL Mathematics (GLM): https://github.com/g-truc/glm
* Plog - portable, simple and extensible C++ logging library: https://github.com/SergiusTheBest/plog
* stb - single-file public domain (or MIT licensed) libraries for C/C++: https://github.com/nothings/stb
* Some Graphics Engine inspiration from https://github.com/mogemimi/pomdog
* Quadtree/Tessellation Terrain for Example/TerrainGame modified from https://github.com/oreonengine/oreon-engine
