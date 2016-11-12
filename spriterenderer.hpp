#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include <GL/glew.h>
#include <memory>
class Shader;
class Sprite;

class SpriteRenderer
{
public:
    SpriteRenderer();

    void draw(const Shader& shader, const Sprite& sprite) const;

private:
    std::shared_ptr<GLuint> VAO, VBO;
};

#endif // SPRITERENDERER_HPP
