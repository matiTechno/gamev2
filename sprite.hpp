#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
class Texture;

std::vector<glm::vec4> loadTexCoords(const std::string& filename);

class Animation
{
public:
    Animation(unsigned int frameDelay, const std::vector<glm::vec4>& texCoordsVec);
    Animation(const Animation&);
    Animation& operator=(const Animation&) = delete;

    const glm::vec4& getFrame() const;
    glm::vec2 getSizeRatio() const;
    bool hasLooped() const;
    void update(unsigned int dt);

private:
    const unsigned int frameDelay;
    const std::vector<glm::vec4> texCoordsVec;
    unsigned int accumulator;
    bool newLoop;
    std::vector<glm::vec4>::const_iterator frame;
    std::vector<glm::vec4>::const_iterator prevFrame;
};

class Sprite
{
public:
    Sprite(const Texture& texture);

    const glm::vec2& getPosition() const;
    const glm::vec2& getSize() const;
    const Texture& getTexture() const;
    const glm::vec4& getTexCoords() const;
    const glm::vec4& getColor() const;
    void setPosition(const glm::vec2& position);
    void setSize(const glm::vec2& size);
    void setTexCoords(const glm::vec4& texCoords);
    void setColor(const glm::vec4& color);

private:
    glm::vec2 position;
    glm::vec2 size;
    const Texture& texture;
    glm::vec4 texCoords;
    glm::vec4 color;
};

class AnimatedSprite: public Sprite
{
public:
    AnimatedSprite(const Texture& texture, const Animation& animation, bool loop);

    void update(unsigned int frameTime);
    bool isActive() const;
    void setActive();

private:
    Animation animation;
    bool loop;
    bool active;
};

#endif // SPRITE_HPP
