#include <sprite.hpp>
#include <texture.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

Sprite::Sprite(const Texture& texture):
    texture(texture),
    color(1.f, 1.f, 1.f, 1.f)

{
    // <empty>
}

const glm::vec2& Sprite::getPosition() const
{
    return position;
}

const glm::vec2& Sprite::getSize() const
{
    return size;
}

const Texture& Sprite::getTexture() const
{
    return texture;
}

const glm::vec4& Sprite::getTexCoords() const
{
    return texCoords;
}

const glm::vec4& Sprite::getColor() const
{
    return color;
}

void Sprite::setPosition(const glm::vec2& position)
{
    this->position = position;
}

void Sprite::setSize(const glm::vec2& size)
{
    this->size = size;
}

void Sprite::setTexCoords(const glm::vec4& texCoords)
{
    this->texCoords = texCoords;
}

void Sprite::setColor(const glm::vec4& color)
{
    this->color = color;
}

AnimatedSprite::AnimatedSprite(const Texture& texture, const Animation& animation, const bool loop):
    Sprite(texture),
    animation(animation),
    loop(loop),
    active(true)
{
    // <empty>
}

void AnimatedSprite::update(const unsigned int frameTime)
{
    animation.update(frameTime);

    if(animation.hasLooped() && !loop)
        active = false;

    else if(animation.getFrame() != getTexCoords())
    {
        setTexCoords(animation.getFrame());
        glm::vec2 sizeRatio = animation.getSizeRatio();
        glm::vec2 prevSize = getSize();
        setSize(prevSize * sizeRatio);
        setPosition(getPosition() + prevSize/2.f - getSize()/2.f);
    }
}

bool AnimatedSprite::isActive() const
{
    return active;
}

void AnimatedSprite::setActive()
{
    active = true;
}

Animation::Animation(const unsigned int frameDelay, const std::vector<glm::vec4>& texCoordsVec):
    frameDelay(frameDelay),
    texCoordsVec(texCoordsVec),
    accumulator(0),
    newLoop(false),
    frame(this->texCoordsVec.cbegin()),
    prevFrame(frame)
{
    // <empty>
}

Animation::Animation(const Animation& rhs):
    frameDelay(rhs.frameDelay),
    texCoordsVec(rhs.texCoordsVec),
    accumulator(0),
    newLoop(false),
    frame(texCoordsVec.cbegin()),
    prevFrame(frame)
{
    // <empty>
}

const glm::vec4& Animation::getFrame() const
{
    return *frame;
}

glm::vec2 Animation::getSizeRatio() const
{
    return glm::vec2(frame->z/prevFrame->z, frame->w/prevFrame->w);
}

bool Animation::hasLooped() const
{
    return newLoop;
}

void Animation::update(const unsigned int dt)
{
    accumulator += dt;

    while(accumulator >= frameDelay)
    {
        prevFrame = frame;
        ++frame;
        if(frame == texCoordsVec.cend())
        {
            frame = texCoordsVec.cbegin();
            newLoop = true;
        }
        accumulator -= frameDelay;
    }
}

std::vector<glm::vec4> loadTexCoords(const std::string& filename)
{
    std::vector<glm::vec4> texCoordsVec;

    std::ifstream ifs(filename);
    if(ifs)
    {
        std::string line;
        while(std::getline(ifs, line))
        {
            if(line.find("spr") != std::string::npos)
            {
                int it = 0;
                glm::vec4 texCoords;
                char dummy;
                std::istringstream iss(line);
                while(iss >> dummy)
                    if(dummy == '"')
                    {
                        switch(it)
                        {
                        case 2: iss >> texCoords.x;
                            break;
                        case 3: iss >> texCoords.y;
                            break;
                        case 4: iss >> texCoords.z;
                            break;
                        case 5: iss >> texCoords.w;
                            break;
                        default:;
                        }
                        ++it;
                        iss >> dummy;
                    }
                texCoordsVec.push_back(texCoords);
            }
        }
    }
    else
        std::cout << "loadTexCoords failed: " << filename << std::endl;
    return texCoordsVec;
}
