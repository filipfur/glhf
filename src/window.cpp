#include "glhf/window.h"

#include "glhf/opengl.h"
#include "glhf/time.h"
#include <cassert>

#ifdef _WIN32
#undef main
#endif

static const int UPDATE_FREQ = 60;
static const int PERIOD_TIME_MS = 1000 / UPDATE_FREQ;
static constexpr float DELTA_TIME_S = 1.0f / static_cast<float>(UPDATE_FREQ);
static glhf::Window *_activeWindow = nullptr;

glhf::Window::Window(IApplication &iApplication) : _iApplication{iApplication} {}

void glhf::_mainLoop() {
    static SDL_Event event;
    static uint32_t lastTick{0};
    static uint32_t deltaTicks{0};
    static uint32_t fpsTime{0};
    static uint16_t fpsCounter{0};
    static float fpsAcc{0};
    while (SDL_PollEvent(&event)) {
        _activeWindow->_running = _activeWindow->_iApplication.event(event);
    }
    uint32_t tick = SDL_GetTicks();
    deltaTicks += (tick - lastTick);
    lastTick = tick;
    if (tick > fpsTime + 100) {
        fpsAcc = fpsAcc * 0.5f + fpsCounter * 5.0f;
        _activeWindow->_iApplication.fps(fpsAcc);
        fpsCounter = 0;
        fpsTime = fpsTime + 100;
    }
    bool updated = false;
    while (deltaTicks >= PERIOD_TIME_MS) {
        if (_activeWindow->_iApplication.update(DELTA_TIME_S)) {
            updated = true;
        }
        deltaTicks -= PERIOD_TIME_MS;
        glhf::time::increment(glhf::time::fromMilliseconds(PERIOD_TIME_MS));
    }
    if (updated) {
        _activeWindow->_iApplication.draw(_activeWindow->_drawableWidth,
                                          _activeWindow->_drawableHeight);
        ++fpsCounter;
        SDL_GL_SwapWindow(_activeWindow->_window);
    }
}

void glhf::Window::run() {
    _activeWindow = this;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(_mainLoop, -1, 1);
#else
    _running = true;
    while (_running) {
        _mainLoop();
        SDL_Delay(1);
    }
#endif
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void glhf::Window::load(const char *title, int width, int height, bool fullscreen) {
    _width = width;
    _height = height;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
    _window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    _glContext = SDL_GL_CreateContext(_window);
    assert(SDL_GL_MakeCurrent(_window, _glContext) == 0);
    SDL_GL_GetDrawableSize(_window, &_drawableWidth, &_drawableHeight);
    if (fullscreen) {
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
    }
#ifndef __EMSCRIPTEN__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    SDL_StopTextInput();
    _iApplication.init(_drawableWidth, _drawableHeight);
}