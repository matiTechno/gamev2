#ifndef SDLWINDOW_HPP
#define SDLWINDOW_HPP

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

// use only if isComplete() = true
class SdlWindow
{
public:
    SdlWindow(const glm::ivec2& size, const char* title);
    ~SdlWindow();
    SdlWindow(const SdlWindow&) = delete;
    SdlWindow& operator=(const SdlWindow&) = delete;

    void clear() const;
    void display() const;
    bool isComplete() const;

    const glm::ivec2 size;

private:
    bool errorFlag;
    SDL_Window* windowID;
    SDL_GLContext glContextID;

    void printOpenglAttr(const char* attrName, SDL_GLattr attr) const;
};

#endif // SDLWINDOW_HPP
