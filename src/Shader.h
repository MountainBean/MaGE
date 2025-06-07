#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>

#include <glad/glad.h>
#include <string_view>
#include <glm/glm.hpp>
#include <vector>

namespace sjd {

enum class SHADER_ERROR {
    badFile,
    vertex,
    fragment,
    geometry,
    program
};

template <typename T>
using ErrShader = std::expected<T, SHADER_ERROR>;


class Shader {
public:
    // the program ID
    Shader(const std::string& vertexPath,
           const std::string& fragmentPath);

    Shader(const std::string& vertexPath,
           const std::string& fragmentPath,
           const std::string& geometryPath);

    auto id() const -> const GLuint& { return m_id; }
    auto isValid() const -> const bool& { return m_isValid; }
    auto errMsg() const -> std::string_view { return m_errMsg; }

    // use/activate the shader
    void use() const {
        if (m_isValid) {
            glUseProgram(m_id);
        } 
        else {
            std::cout << "ERROR::SHADER::INVALID\n";
            std::cout << m_errMsg;
        }
    }

    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec3& vec) const;
    void setUniform(const std::string& name, const glm::mat4& mat) const;

protected:
    GLuint m_id;
    uint16_t m_vertexAttributes;
    bool m_isValid;
    std::string m_errMsg {"No Error Message"};

    auto _loadShaderFile(const std::string& path) -> ErrShader<std::string>;
    enum ShaderType {
        vertex = GL_VERTEX_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        fragment = GL_FRAGMENT_SHADER
    };
    auto _compileSubShader(ShaderType shaderType,
                            std::string shaderSource) -> ErrShader<GLuint>;
    bool _compileProgram(GLuint vertShader, GLuint fragShader);
    bool _compileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader);
};

}
#endif
