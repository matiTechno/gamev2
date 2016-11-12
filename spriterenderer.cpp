#include <spriterenderer.hpp>
#include <shader.hpp>
#include <sprite.hpp>
#include <glm/gtx/transform.hpp>
#include <texture.hpp>

SpriteRenderer::SpriteRenderer()
{
    // note: texture is flipped by stbi
    GLfloat vertices[] =
    {
        // position / texCoords
        0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 0.f, 1.f,
        0.f, 0.f, 0.f, 0.f
    };

    VAO.reset(new GLuint, [](GLuint* VAO)
    {
        glDeleteVertexArrays(1, VAO);
        delete VAO;
    });

    VBO.reset(new GLuint, [](GLuint* VBO)
    {
        glDeleteBuffers(1, VBO);
        delete VBO;
    });

    glGenBuffers(1, VBO.get());
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, VAO.get());
    glBindVertexArray(*VAO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void SpriteRenderer::draw(const Shader& shader, const Sprite& sprite) const
{
    shader.bind();

    glm::mat4 model;
    // #2 set position
    model = glm::translate(model, glm::vec3(sprite.getPosition(), 0.f));
    // #1 scale
    model = glm::scale(model, glm::vec3(sprite.getSize(), 1.f));

    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4f(glGetUniformLocation(shader.getID(), "spriteColor"), sprite.getColor().x, sprite.getColor().y, sprite.getColor().z, sprite.getColor().w);


    sprite.getTexture().bind();

    glm::vec2 texSize(static_cast<GLfloat>(sprite.getTexCoords().z)/static_cast<GLfloat>(sprite.getTexture().getSize().x),
                      static_cast<GLfloat>(sprite.getTexCoords().w)/static_cast<GLfloat>(sprite.getTexture().getSize().y));

    glm::vec2 texShift(static_cast<GLfloat>(sprite.getTexCoords().x)/static_cast<GLfloat>(sprite.getTexture().getSize().x),
                       static_cast<GLfloat>(sprite.getTexCoords().y)/static_cast<GLfloat>(sprite.getTexture().getSize().y));

    glUniform2f(glGetUniformLocation(shader.getID(), "texSize"), texSize.x, texSize.y);
    glUniform2f(glGetUniformLocation(shader.getID(), "texShift"), texShift.x, texShift.y);

    glBindVertexArray(*VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
