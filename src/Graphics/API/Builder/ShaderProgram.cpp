#include <iostream> // std::cerr

#include <Graphics/API/Builder/ShaderProgram.hpp> // Graphics::API::Builder::ShaderProgram

namespace Graphics {
namespace API {
namespace Builder {

ShaderProgram::~ShaderProgram() {
    for (auto& shaderFile: _shadersFiles) {
        shaderFile.second.close();
    }
}

bool ShaderProgram::build(API::ShaderProgram& shaderProgram) {
    GLuint glShaderProgram = glCreateProgram();
    if (!glShaderProgram) {
        // TODO: replace this with logger
        std::cerr << "Builder::ShaderProgram::build: Can't create shader program" << std::endl;
        return false;
    }

    std::unordered_map<GLenum, GLuint> shaders;
    for (auto& it: _shadersFiles) {
        auto& shaderFile = it.second;
        auto shaderType = it.first;

        // Read shader file content
        // TODO: use a resource manager
        shaderFile.seekg(0, shaderFile.end);
        int fileSize = static_cast<int>(shaderFile.tellg());
        shaderFile.seekg(0, shaderFile.beg);
        char* shaderContent = new char[fileSize + 1];
        std::memset(shaderContent, 0, fileSize + 1);

        shaderFile.read(shaderContent, fileSize);

        GLuint shader = glCreateShader(shaderType);
        if (!shader) {
            // TODO: replace this with logger
            std::cerr << "Builder::ShaderProgram::build: Can't create shader" << std::endl;
            return false;
        }

        glShaderSource(shader, 1, &shaderContent, NULL);
        glCompileShader(shader);

        delete[] shaderContent;

        if (!checkShaderStatus(shader, GL_COMPILE_STATUS)) {
            return false;
        }

        glAttachShader(glShaderProgram, shader);

        shaders[shaderType] = shader;
    }

    glLinkProgram(glShaderProgram);
    if (!checkProgramStatus(glShaderProgram, GL_LINK_STATUS)) {
        return false;
    }

    shaderProgram = API::ShaderProgram(glShaderProgram, shaders);

    return true;
}

bool ShaderProgram::setShader(GLenum shaderType, const std::string& shaderFileName) {
    // TODO: replace resource manager
    std::ifstream file(shaderFileName.c_str());

    if (!file.good()) {
        // TODO: replace this with logger
        std::cerr << "Builder::ShaderProgram::setShader: Can't open shader file \"" << shaderFileName << "\"" << std::endl;
        return false;
    }

    // The shader has already been set, close the file
    if (_shadersFiles.find(shaderType) != _shadersFiles.end()) {
        _shadersFiles[shaderType].close();
    }

    _shadersFiles[shaderType] = std::move(file);
    return true;
}

bool ShaderProgram::checkShaderStatus(GLuint shader, GLenum statusName) {
    GLint success;
    glGetShaderiv(shader, statusName, &success);

    // If compilation did not succeed, get the error message and throw an Exception
    if (!success)
    {
        GLchar infoLog[512] = {};
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        // TODO: replace this with logger
        std::cerr << "ShaderProgram::checkShaderError: Error: " << infoLog << std::endl;
        return false;
    }

    return true;
}

bool ShaderProgram::checkProgramStatus(GLuint shaderProgram, GLenum statusName) {
    GLint success;
    glGetProgramiv(shaderProgram, statusName, &success);

    // If compilation did not succeed, get the error message and throw an Exception
    if (!success)
    {
        GLchar infoLog[512] = {};
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        // TODO: replace this with logger
        std::cerr << "ShaderProgram::checkProgramError: Error: " << infoLog << std::endl;
        return false;
    }

    return true;
}

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
