//
// Created by dragos on 30.04.21.
//

#ifndef HAND_SHADER_H
#define HAND_SHADER_H

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int m_RendererId;
    std::string m_filePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    int getUniformLocation(const std::string& name);
    static unsigned int createShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
    static unsigned int compileShader(unsigned int type, const std::string& source);
    static ShaderSources parseShader(const std::string& filepath);
public:
    Shader(const std::string&  filePath);
    ~Shader();
    void Bind() const;
    void Unbind() const;
    unsigned int getAttributeLocation(std::string name);
    // Set uniforms
    void setUniform1i(const std::string &name, int val);
    void setUniform1iv(const std::string& name, int count, const int* val);
    void setUniform1f(const std::string &name, float val);
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string &name, const glm::mat4& value);
};
#endif //HAND_SHADER_H
