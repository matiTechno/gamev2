#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <memory>
#include <GL/glew.h>

// use only if isComplete() = true
// file extensions: vs, fs, gs
class Shader
{
public:
    Shader(const std::string& filenameNoExt, bool isGs = false);

    void bind() const;
    GLuint getID() const;
    bool isComplete() const;

private:
    bool errorFlag;
    std::shared_ptr<GLuint> programID;
    static GLuint boundProgram;

    std::string load(const std::string& filename) const;
    GLuint compile(const std::string& data, GLenum shaderType) const;
    bool getError(bool isProgram, GLuint ID, GLenum flag, const std::string& errorMsg) const;
};

#endif // SHADER_HPP
