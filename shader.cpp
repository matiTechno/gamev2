#include <shader.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

GLuint Shader::boundProgram = 0;

Shader::Shader(const std::string& filenameNoExt, bool isGs):
    errorFlag(true)
{
    GLuint vsID = compile(load(filenameNoExt + ".vs"), GL_VERTEX_SHADER);
    GLuint fsID = compile(load(filenameNoExt + ".fs"), GL_FRAGMENT_SHADER);
    GLuint gsID = 0;
    if(isGs)
        gsID = compile(load(filenameNoExt + ".gs"), GL_GEOMETRY_SHADER);

    bool compileErrors = getError(false, vsID, GL_COMPILE_STATUS, "Shader::compile failed: " + filenameNoExt + ".vs")
            || getError(false, fsID, GL_COMPILE_STATUS, "Shader::compile failed: " + filenameNoExt + ".fs");

    if(isGs)
        compileErrors = compileErrors || getError(false, gsID, GL_COMPILE_STATUS, "Shader::compile failed: " + filenameNoExt + ".gs");

    if(!compileErrors)
    {
        GLuint temp = glCreateProgram();
        glAttachShader(temp, vsID);
        glAttachShader(temp, fsID);
        if(isGs)
            glAttachShader(temp, gsID);

        glLinkProgram(temp);

        if(getError(true, temp, GL_LINK_STATUS, "glLinkProgram failed: " + filenameNoExt))
            glDeleteProgram(temp);
        else
        {
            glDetachShader(temp, vsID);
            glDetachShader(temp, fsID);
            if(isGs)
                glDetachShader(temp, gsID);

            programID.reset(new GLuint(temp), [](GLuint* programID)
            {
                glDeleteProgram(*programID);
                delete programID;
            });

            errorFlag = false;
        }
    }
    glDeleteShader(vsID);
    glDeleteShader(fsID);
    if(isGs)
        glDeleteShader(gsID);
}

void Shader::bind() const
{
    if(*programID != boundProgram)
    {
        glUseProgram(*programID);
        boundProgram = *programID;
    }
}

GLuint Shader::getID() const
{
    return *programID;
}

bool Shader::isComplete() const
{
    return !errorFlag;
}

std::string Shader::load(const std::string& filename) const
{
    std::ifstream fs(filename);
    if(fs)
    {
        std::stringstream ss;
        ss << fs.rdbuf();
        return ss.str();
    }
    std::cout << "Shader::load failed: " << filename << std::endl;
    return std::string();
}

GLuint Shader::compile(const std::string& data, const GLenum shaderType) const
{
    GLuint shaderID = glCreateShader(shaderType);
    const GLchar* source = data.c_str();
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);
    return shaderID;
}

bool Shader::getError(const bool isProgram, const GLuint ID, const GLenum flag, const std::string& errorMsg) const
{
    GLint success = 0;
    if(isProgram)
        glGetProgramiv(ID, flag, &success);
    else
        glGetShaderiv(ID, flag, &success);

    if(success == GL_FALSE)
    {
        GLint length;
        if(isProgram)
            glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
        else
            glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> infoLog(static_cast<std::vector<GLchar>::size_type>(length));
        if(isProgram)
            glGetProgramInfoLog(ID, length, nullptr, infoLog.data());
        else
            glGetShaderInfoLog(ID, length, nullptr, infoLog.data());

        std::cout << errorMsg << std::endl;
        for(const auto i: infoLog)
            std::cout << i;
        std::cout << std::endl;
        return true;
    }
    return false;
}
