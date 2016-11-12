#include <game.hpp>
#include <SDL2/SDL.h>
#include <sdlwindow.hpp>

Game::Game(const SdlWindow& window):
    window(window),
    input(),
    isRunning(true)
{
    //temp
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_FRAMEBUFFER_SRGB);

    textures.push_back(Texture("res/demo_tex.png", MinFilter::nearest, MagFilter::nearest, Wrap::clampToEdge, Wrap::clampToEdge, true));
    shaders.push_back(Shader("res/sprite_shader", false));

    std::vector<glm::vec4> texCoordsVec= loadTexCoords("res/demo_tex.coords");
    if(!texCoordsVec.size())
        return;
    sprite = std::unique_ptr<AnimatedSprite>(std::make_unique<AnimatedSprite>(textures[0], Animation(120, texCoordsVec), true));
    sprite->setPosition(glm::vec2(100.f, 100.f));
    sprite->setSize(glm::vec2(30.f, 30.f));

    // game coordinates
    // [0,0]           [win_width, 0]
    // [0, height] [win_width, win_height]
    glm::mat4 projection = glm::ortho(0.f, float(window.size.x), float(window.size.y), 0.f);
    shaders[0].bind();
    glUniformMatrix4fv(glGetUniformLocation(shaders[0].getID(), "projection"), 1, GL_FALSE, &projection[0][0]);

    for(const auto& texture: textures)
        if(!texture.isComplete())
            return;
    for(const auto& shader: shaders)
        if(!shader.isComplete())
            return;
    //end
    enterLoop();
}

void Game::enterLoop()
{
    const unsigned int dt = 4;
    unsigned int currentTime = SDL_GetTicks();
    unsigned int accumulator = 0;

    while(isRunning)
    {
        processInput();

        unsigned int newTime = SDL_GetTicks();
        unsigned int frameTime = newTime - currentTime;
        if(frameTime > 30)
            frameTime = 30;

        currentTime = newTime;

        accumulator += frameTime;

        while(accumulator >= dt)
        {
            update(dt);
            accumulator -= dt;
        }
        // to do: interpolation
        //temp
        updateGraphics(frameTime);
        //end
        render();
    }
}

void Game::processInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            isRunning = false;

        else if(event.type == SDL_KEYDOWN && !event.key.repeat)
            input.keyDownEvent(event);

        else if(event.type == SDL_KEYUP && !event.key.repeat)
            input.keyUpEvent(event);
    }

    if(input.wasKeyPressed(SDLK_ESCAPE))
        isRunning = false;
}

void Game::update(const unsigned int dt)
{
    (void)dt;
}

void Game::updateGraphics(const unsigned int frameTime)
{
    if(sprite->isActive())
        sprite->update(frameTime);
}

void Game::render() const
{
    window.clear();
    //temp
    if(sprite->isActive())
        renderer.draw(shaders[0], *sprite);
    //end
    window.display();
}
