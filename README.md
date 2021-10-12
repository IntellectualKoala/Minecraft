# Minecraft
A Minecraft-clone written in C++/Vulkan

## Current state
It is currently very bare-bones.

## Planned features
* Textures
* Procedural generation
* Collision detection
* Etc.

## How to build
1. Make sure the Vulkan SDK is installed. ([You can get the SDK here](https://vulkan.lunarg.com/))
2. Clone the repository with git (make sure to use the --recursive flag to initialize the submodules)
3. Build with cmake however you like.

Your commands should look roughly like this:
```
git clone https://github.com/IntellectualKoala/Minecraft --recursive
cmake Minecraft -B Minecraft/build
```

Note: Make sure the compiled `res` folder is in your executable's working directory, otherwise the binary won't be able to locate the resources.
