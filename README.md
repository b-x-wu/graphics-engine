# Graphics Engine

I'm trying my hand at a graphics engine in C++. I have little experience in the language and almost no knowledge of computer graphics, but I'm trying my best. I'm working out of [Fundamentals of Computer Graphics, Fourth Edition by Peter Shirley and Steve Marschner](https://github.com/t4world/Computer-Graphics/blob/master/Fundamentals-of-Computer-Graphics-Fourth-Edition.pdf).

Wish me luck.

So far, this program encompasses everything from Chapter 4 on Ray Tracing. The following render uses Blinn-Phong and Lambert shading on the spheres and a modified form of "Ideal Specular Shading" for the plane.

![Rendering of two spheres and a plane](./sample_images/chapter2.bmp)

## Dev Notes

Current command to create the WASM binaries:

```
em++ -lembind main.cpp camera.cpp lightSource.cpp math.cpp scene.cpp shader.cpp surface.cpp -s MODULARIZE=1 -sNO_DISABLE_EXCEPTION_CATCHING --no-entry -s WASM=1 -s EXPORT_NAME="GRAPHICS_ENGINE" -s ENVIRONMENT="web" -o graphics_engine.js -sALLOW_MEMORY_GROWTH -O2 --profiling-funcs -g
```

Have a look at [this article](https://blog.esciencecenter.nl/using-c-in-a-web-app-with-webassembly-efd78c08469).
