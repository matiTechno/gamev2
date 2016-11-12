#ifndef INPUT_HPP
#define INPUT_HPP

#include <unordered_map>
#include <SDL2/SDL.h>

class Input
{
public:
    void clearStates();
    void keyDownEvent(const SDL_Event& event);
    void keyUpEvent(const SDL_Event& event);
    bool wasKeyPressed(SDL_Keycode key) const;
    bool wasKeyReleased(SDL_Keycode key) const;
    bool isKeyPressed(SDL_Keycode key) const;

private:
    std::unordered_map<SDL_Keycode, bool> keys1;
    std::unordered_map<SDL_Keycode, bool> keys2;
    std::unordered_map<SDL_Keycode, bool> keys3;
};

#endif // INPUT_HPP
