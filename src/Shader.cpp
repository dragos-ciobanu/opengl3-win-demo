//
// Created by dragos on 30.04.21.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "Renderer.h"


int Shader::getUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) == m_UniformLocationCache.end()) {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        if (location == -1) std::cout << "Uniform \"" << name << "\" doesn't exist!" << std::endl;
        m_UniformLocationCache[name] = location;
    }

    return m_UniformLocationCache[name];
};

Shader::Shader(const std::string& filePath)
:m_filePath(filePath), m_RendererId(0)
{
    ShaderSources sources = parseShader(filePath);
//    std::cout << "VERTEX" << std::endl;
//    std::cout << sources.VertexSource << std::endl;
//    std::cout << "FRAGMENT" << std::endl;
//    std::cout << sources.FragmentSource << std::endl;

    m_RendererId = createShader(sources.VertexSource, sources.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

ShaderSources Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType  {NONE = -1, VERTEX = 0, FRAGMENT = 1};

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << std::endl;
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int  Shader::createShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderCode);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    GLCall(glAttachShader(program, vertexShader));
    GLCall(glAttachShader(program, fragmentShader));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
    GLCall(glDetachShader(program, vertexShader));
    GLCall(glDetachShader(program, fragmentShader));

    return program;
}

void Shader::setUniform1f(const std::string &name, float val) {
    GLCall(glUniform1f(getUniformLocation(name), val));
}

void Shader::setUniform1i(const std::string &name, int val) {
    GLCall(glUniform1i(getUniformLocation(name), val));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::getAttributeLocation(std::string name) {
     return glGetAttribLocation(m_RendererId, name.c_str());
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &value) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]));
};

