#include <input.hpp>

void Input::clearStates()
{
    keys1.clear();
    keys2.clear();
}

void Input::keyDownEvent(const SDL_Event& event)
{
    keys1[event.key.keysym.sym] = true;
    keys3[event.key.keysym.sym] = true;
}

void Input::keyUpEvent(const SDL_Event& event)
{
    keys2[event.key.keysym.sym] = true;
    keys3.erase(event.key.keysym.sym);
}

bool Input::wasKeyPressed(const SDL_Keycode key) const
{
    auto i = keys1.find(key);

    if(i == keys1.end())
        return false;
    else
        return true;
}

bool Input::wasKeyReleased(const SDL_Keycode key) const
{
    auto i = keys2.find(key);

    if(i == keys2.end())
        return false;
    else
        return true;
}

bool Input::isKeyPressed(const SDL_Keycode key) const
{
    auto i = keys3.find(key);

    if(i == keys3.end())
        return false;
    else
        return true;
}
