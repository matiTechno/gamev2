#include <texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

GLuint Texture::boundTexture = 0;

Texture::Texture(const std::string& filename, const MinFilter minFilter, const MagFilter magFilter, const Wrap wrapS, const Wrap wrapT, const bool is_sRGB):
    errorFlag(true)
{
    unsigned char* data = stbi_load(filename.c_str(), &size.x, &size.y, nullptr, 4);

    if(data)
    {
        textureID.reset(new GLuint, [](GLuint* textureID)
        {
            glDeleteTextures(1, textureID);
            delete textureID;
        });

        glGenTextures(1, textureID.get());
        glBindTexture(GL_TEXTURE_2D, *textureID);
        boundTexture = *textureID;

        if(is_sRGB)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        if(minFilter == MinFilter::nearestMipmapNearest || minFilter == MinFilter::linearMipmapNearest ||
                minFilter == MinFilter::nearestMipmapLinear || minFilter == MinFilter::linearMipmapLinear)
            glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));

        stbi_image_free(data);

        errorFlag = false;
    }
    else
        std::cout << "stbi_load failed: " << filename << std::endl;
}

void Texture::bind(const GLuint unit) const
{
    if(*textureID != boundTexture)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, *textureID);
        boundTexture = *textureID;
    }
}

const glm::ivec2& Texture::getSize() const
{
    return size;
}

bool Texture::isComplete() const
{
    return !errorFlag;
}
