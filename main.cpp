#include <sdlwindow.hpp>
#include <game.hpp>
#include <iostream>

int main()
{
    SdlWindow window(glm::ivec2(400, 400), "SDL_OpenGL_arkanoid");
    if(!window.isComplete())
        goto end;
    {
        Game game(window);
        (void)game;
    }
end:
    std::cout << "returned from main" << std::endl;
    return 0;
}
