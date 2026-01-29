#include "glhf/window.h"

#include <cassert>

#ifdef _WIN32
#undef main
#endif

static const int UPDATE_FREQ = 60;
static const int PERIOD_TIME_MS = 1000 / UPDATE_FREQ;
static constexpr float DELTA_TIME_S = 1.0f / static_cast<float>(UPDATE_FREQ);

static bool _running;
static glhf::IApplication *_iApplication;
int glhf::Window::WIDTH;
int glhf::Window::HEIGHT;
int glhf::Window::BUFFER_WIDTH;
int glhf::Window::BUFFER_HEIGHT;
static SDL_Window *_window{nullptr};
static SDL_GLContext _glContext{nullptr};

static void _mainLoop() {
    static SDL_Event event;
    static uint32_t lastTick{0};
    static uint32_t deltaTicks{0};
    static uint32_t fpsTime{0};
    static uint16_t fpsCounter{0};
    static float fpsAcc{0};
    while (SDL_PollEvent(&event)) {
        _running = _iApplication->event(event);
    }
    uint32_t tick = SDL_GetTicks();
    deltaTicks += (tick - lastTick);
    lastTick = tick;
    if (tick > fpsTime + 100) {
        fpsAcc = fpsAcc * 0.5f + fpsCounter * 5.0f;
        _iApplication->fps(fpsAcc);
        fpsCounter = 0;
        fpsTime = fpsTime + 100;
    }
    bool updated = false;
    while (deltaTicks >= PERIOD_TIME_MS) {
        if (_iApplication->update(DELTA_TIME_S)) {
            updated = true;
        }
        deltaTicks -= PERIOD_TIME_MS;
        glhf::Time::increment(glhf::Time::fromMilliseconds(PERIOD_TIME_MS));
    }
    if (updated) {
        _iApplication->draw(glhf::Window::BUFFER_WIDTH, glhf::Window::BUFFER_HEIGHT);
        ++fpsCounter;
        SDL_GL_SwapWindow(_window);
    }
}

void glhf::Window::run() {
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

void glhf::Window::create(IApplication *iApplication, const char *title, int width, int height,
                          bool fullscreen) {
    _iApplication = iApplication;
    WIDTH = width;
    HEIGHT = height;
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
    SDL_GL_GetDrawableSize(_window, &BUFFER_WIDTH, &BUFFER_HEIGHT);
    if (fullscreen) {
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
    }
#ifndef __EMSCRIPTEN__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    SDL_StopTextInput();
    _iApplication->init(BUFFER_WIDTH, BUFFER_HEIGHT);
}