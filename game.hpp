#ifndef GAME_HPP
#define GAME_HPP

class SdlWindow;
#include <input.hpp>

//temp
#include <texture.hpp>
#include <shader.hpp>
#include <spriterenderer.hpp>
#include <sprite.hpp>
#include <memory>
#include <glm/gtx/transform.hpp>
#include <vector>
//end

class Game
{
public:
    Game(const SdlWindow& window);

private:
    const SdlWindow& window;
    Input input;
    bool isRunning;

    void enterLoop();
    void processInput();
    void update(unsigned int dt);
    //temp
    void updateGraphics(unsigned int frameTime);
    //end
    void render() const;

    //temp
    std::vector<Texture> textures;
    std::vector<Shader> shaders;
    SpriteRenderer renderer;
    std::unique_ptr<AnimatedSprite> sprite;
    //end
};

#endif // GAME_HPP
