#pragma once

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
#include <GLES3/gl32.h>
#define GLHF_GLSL_VERSION "#version 300 es"
#else
#include <GL/glew.h>
#define GLHF_GLSL_VERSION "#version 330 core"
#endif