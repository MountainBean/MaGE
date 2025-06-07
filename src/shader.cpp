#include <shader.h>

namespace sjd {

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
: m_isValid {false}
{
    // Attempt to read shader files into memory
    auto vertexCode {_loadShaderFile(vertexPath)};
    if (!vertexCode.has_value()){
        std::cout << "Failed to load vertex shader at: " << vertexPath << "\n";
        return;
    }
    auto fragmentCode {_loadShaderFile(fragmentPath)};
    if (!fragmentCode.has_value()){
        std::cout << "Failed to load fragment shader at: " << fragmentPath << "\n";
        return;
    }

    auto vertShader {_compileSubShader(ShaderType::vertex, vertexCode.value())};
    if (!vertShader.has_value()){
        std::cout << "Failed to compile vertex shader.\n";
        return;
    }
    auto fragShader {_compileSubShader(ShaderType::fragment, fragmentCode.value())};
    if (!fragShader.has_value()){
        std::cout << "Failed to compile fragment shader.\n";
        return;
    }

    if (!_compileProgram(vertShader.value(), fragShader.value())) {
        std::cout << "Failed to link shader program.\n";
        return;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertShader.value());
    glDeleteShader(fragShader.value());
    m_isValid = true;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
: m_isValid {false}
{
    // Attempt to read shader files into memory
    auto vertexCode {_loadShaderFile(vertexPath)};
    if (!vertexCode.has_value()){
        std::cout << "Failed to load vertex shader at: " << vertexPath << "\n";
        return;
    }
    auto fragmentCode {_loadShaderFile(fragmentPath)};
    if (!fragmentCode.has_value()){
        std::cout << "Failed to load fragment shader at: " << fragmentPath << "\n";
        return;
    }
    auto geometryCode {_loadShaderFile(geometryPath)};
    if (!geometryCode.has_value()){
        std::cout << "Failed to load geometry shader at: " << geometryPath << "\n";
        return;
    }

    auto vertShader {_compileSubShader(ShaderType::vertex, vertexCode.value())};
    if (!vertShader.has_value()){
        std::cout << "Failed to compile vertex shader.\n";
        return;
    }
    auto fragShader {_compileSubShader(ShaderType::fragment, fragmentCode.value())};
    if (!fragShader.has_value()){
        std::cout << "Failed to compile fragment shader.\n";
        return;
    }
    auto geomShader {_compileSubShader(ShaderType::geometry, geometryCode.value())};
    if (!geomShader.has_value()){
        std::cout << "Failed to compile geometry shader.\n";
        return;
    }

    if (!_compileProgram(vertShader.value(),
                        fragShader.value(),
                        geomShader.value())) {
        std::cout << "Failed to link shader program.\n";
        return;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertShader.value());
    glDeleteShader(geomShader.value());
    glDeleteShader(fragShader.value());
    m_isValid = true;
}

void Shader::setUniform(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}

void Shader::setUniform(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}

void Shader::setUniform(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); 
}

void Shader::setUniform(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &vec[0]);
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

auto Shader::_loadShaderFile(const std::string& path) -> ErrShader<std::string> {
    // Attempt to read shader files into memory
    std::string shaderSource;
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    // | badbit     irrecoverable stream error
    // | failbit    input/output operation failed (formatting or extraction error)
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        shaderFile.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderSource = shaderStream.str();
    }
    catch(std::ifstream::failure e) {
        m_errMsg = "ERROR::SHADER::FILE_READ_FAILED:\n" + path + "\n";
        return std::unexpected(SHADER_ERROR::badFile);
    }
    return shaderSource;
}

auto Shader::_compileSubShader(ShaderType shaderType,
                               std::string shaderSource) -> ErrShader<GLuint> {

    GLuint shaderId {};
    int success {};

    const char* shaderCode = shaderSource.c_str();
    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);
    // print compile errors if any
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success) {
        std::string shaderName;
        SHADER_ERROR err;
        switch (shaderType) {
        case vertex:
            err = SHADER_ERROR::vertex;
            shaderName = "VECTOR";
            break;
        case fragment:
            err = SHADER_ERROR::fragment;
            shaderName = "FRAGMENT";
            break;
        case geometry:
            err = SHADER_ERROR::geometry;
            shaderName = "GEOMETRY";
            break;
        }
        GLint logSize = 6;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
        // The logSize includes the NULL character
	std::vector<GLchar> infoLog(logSize);
        glGetShaderInfoLog(shaderId, logSize, &logSize, infoLog.data());
        std::string errorString(infoLog.begin(), infoLog.end());
        m_errMsg = "ERROR::SHADER::" + shaderName + "::COMPILATION_FAILED\n" + errorString + "\n";
        return std::unexpected(err);
    };

    return shaderId;
}

bool Shader::_compileProgram(GLuint vertShader, GLuint fragShader) {
    int success {};
    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertShader);
    glAttachShader(m_id, fragShader);
    glLinkProgram(m_id);

    // print linking errors if any
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success) {
        GLint logSize = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
	std::vector<GLchar> infoLog(logSize);
        glGetProgramInfoLog(m_id, logSize, &logSize, infoLog.data());
        std::string errorString(infoLog.begin(), infoLog.end());
        m_errMsg = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + errorString + "\n";
        return false;
    }
    return true;
}

bool Shader::_compileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader) {
    int success {};
    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertShader);
    glAttachShader(m_id, geomShader);
    glAttachShader(m_id, fragShader);
    glLinkProgram(m_id);

    // print linking errors if any
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        GLint logSize = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
	std::vector<GLchar> infoLog(logSize);
        glGetProgramInfoLog(m_id, logSize, &logSize, infoLog.data());
        std::string errorString(infoLog.begin(), infoLog.end());
        m_errMsg = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + errorString + "\n";
        return false;
    }
    return true;
}

}
