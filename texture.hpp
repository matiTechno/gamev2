#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <memory>

enum class MinFilter
{
    nearest = GL_NEAREST,
    linear = GL_LINEAR,
    nearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    linearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    nearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    linearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class MagFilter
{
    nearest = GL_NEAREST,
    linear = GL_LINEAR,
};

enum class Wrap
{
    clampToEdge = GL_CLAMP_TO_EDGE,
    //clampToBorder = GL_CLAMP_TO_BORDER,
    mirroredRepeat = GL_MIRRORED_REPEAT,
    repeat = GL_REPEAT,
    mirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
};

// use only if isComplete() = true
class Texture
{
public:

    Texture(const std::string& filename, MinFilter minFilter, MagFilter magFilter, Wrap wrapS, Wrap wrapT, bool is_sRGB);

    void bind(GLuint unit = 0) const;
    const glm::ivec2& getSize() const;
    bool isComplete() const;

private:
    bool errorFlag;
    std::shared_ptr<GLuint> textureID;
    glm::ivec2 size;
    static GLuint boundTexture;
};

#endif // TEXTURE_HPP
