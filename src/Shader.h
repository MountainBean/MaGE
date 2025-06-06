#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <string_view>
#include <glm/glm.hpp>
#include <vector>

namespace sjd {

class Shader {
public:
    // the program ID
    Shader(const std::string& vertexPath,
           const std::string& fragmentPath);

    Shader(const std::string& vertexPath,
           const std::string& fragmentPath,
           const std::string& geometryPath);

    // use/activate the shader
    void use() {
        if (m_isValid) {
            glUseProgram(m_id);
        } 
        else {
            std::cout << "ERROR::SHADER::INVALID\n";
        }
    }

    bool isValid() {return m_isValid;}

    struct Uniforms {};

    void draw(Uniforms uniforms);

    template <typename T>
    void setUniform(const std::string& name, T value) const;

protected:
    GLuint m_id;
    uint16_t m_vertexAttributes;
    bool m_isValid;

    std::string _loadShaderFile(const std::string& path);
    enum ShaderType {
        vector = GL_VERTEX_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        fragment = GL_FRAGMENT_SHADER
    };
    GLuint _compileSubShader(ShaderType shaderType,
                            std::string shaderSource);
    void _compileProgram(GLuint vertShader, GLuint fragShader);
    void _compileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader);
};

}
#endif
