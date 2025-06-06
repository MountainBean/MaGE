#include <shader.h>

namespace sjd {

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{

    // Attempt to read shader files into memory
    std::string vertexCode {_loadShaderFile(vertexPath)};
    std::string fragmentCode {_loadShaderFile(fragmentPath)};

    GLuint vertShader {_compileSubShader(ShaderType::vector, vertexCode)};
    GLuint fragShader {_compileSubShader(ShaderType::fragment, fragmentCode)};

    _compileProgram(vertShader, fragShader);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    m_isValid = true;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{

    // Attempt to read shader files into memory
    std::string vertexCode {_loadShaderFile(vertexPath)};
    std::string fragmentCode {_loadShaderFile(fragmentPath)};
    std::string geometryCode {_loadShaderFile(geometryPath)};

    GLuint vertShader {_compileSubShader(ShaderType::vector, vertexCode)};
    GLuint fragShader {_compileSubShader(ShaderType::fragment, fragmentCode)};
    GLuint geomShader {_compileSubShader(ShaderType::geometry, geometryCode)};

    _compileProgram(vertShader, fragShader, geomShader);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertShader);
    glDeleteShader(geomShader);
    glDeleteShader(fragShader);
    m_isValid = true;
}

std::string Shader::_loadShaderFile(const std::string& path) {
    // Attempt to read shader files into memory
    std::string shaderSource;
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    // | badbit     irrecoverable stream error
    // | failbit    input/output operation failed (formatting or extraction error)
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
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
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }
    return shaderSource;
}

GLuint Shader::_compileSubShader(ShaderType shaderType, std::string shaderSource) {

    GLuint shaderId {};
    int success {};
    char infoLog[512];

    const char* shaderCode = shaderSource.c_str();
    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);
    // print compile errors if any
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        std::string shaderName;
        switch (shaderType) {
        case vector:
            shaderName = "VECTOR";
            break;
        case fragment:
            shaderName = "FRAGMENT";
            break;
        case geometry:
            shaderName = "GEOMETRY";
            break;
        }
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n" << infoLog << "\n";
    };

    return shaderId;
}

void Shader::_compileProgram(GLuint vertShader, GLuint fragShader) {
    int success {};
    char infoLog[512];
    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertShader);
    glAttachShader(m_id, fragShader);
    glLinkProgram(m_id);

    // print linking errors if any
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::_compileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader) {
    int success {};
    char infoLog[512];
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
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

template <>
void Shader::setUniform<bool>(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}

template <>
void Shader::setUniform<int>(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}

template <>
void Shader::setUniform<float>(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); 
}

template <>
void Shader::setUniform<const glm::vec3&>(const std::string &name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &vec[0]);
}

template <>
void Shader::setUniform<const glm::mat4&>(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

}
