#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#include <emscripten/emscripten.h>
#else
#include <SDL.h>
#endif

namespace glhf {

struct IApplication {
    /// @brief called once when the window is loaded
    /// @param width default framebuffer width
    /// @param height default framebuffer height
    virtual void init(int width, int height) = 0;
    /// @brief called when there is new fps info
    /// @param frames number of frames per second
    virtual void fps(float frames) {}
    /// @brief called when its time to update
    /// @param dt time since last update (delta)
    /// @return true if application was updated
    virtual bool update(float dt) = 0;
    /// @brief called when it is time to render new frame
    virtual void draw(int width, int height) = 0;
    /*
      SDL_KEYDOWN
      SDL_KEYUP
      - ev.key.keysym.sym,
      - ev.key.keysym.mod

      SDL_MOUSEBUTTONDOWN
      SDL_MOUSEBUTTONUP
      - ev.button.button
      - ev.button.x/y

      SDL_MOUSEWHEEL
      - ev.wheel.preciseX/Y

      SDL_MOUSEMOTION
      - ev.motion.x/y
      - ev.motion.xrel/yrel

      SDL_TEXTINPUT
      - ev.text.text

      SDL_QUIT

      @return false if quitting
    */
    virtual bool event(const SDL_Event &ev) {
        switch (ev.type) {
        case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
            break;
        case SDL_QUIT:
            return false;
        default:
            break;
        }
        return true;
    };
};

struct Window {
    explicit Window(IApplication &iApplication);
    void load(const char *title, int width, int height, bool fullscreen);
    void run();

  protected:
    friend void _mainLoop();
    bool _running;
    IApplication &_iApplication;
    int _width;
    int _height;
    int _drawableWidth;
    int _drawableHeight;
    SDL_Window *_window{nullptr};
    SDL_GLContext _glContext{nullptr};
};
} // namespace glhf