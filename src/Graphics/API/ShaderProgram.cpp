#include <Graphics/API/ShaderProgram.hpp>

namespace Graphics {
namespace API {

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) {
    _shaderProgram = shaderProgram._shaderProgram;
    _shaders = shaderProgram._shaders;

    shaderProgram._shaderProgram = 0;
    shaderProgram._shaders.clear();
}


ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) {
    _shaderProgram = shaderProgram._shaderProgram;
    _shaders = shaderProgram._shaders;

    shaderProgram._shaderProgram = 0;
    shaderProgram._shaders.clear();

    return *this;
}

ShaderProgram::~ShaderProgram() {
    if (_shaderProgram) {
        glDeleteProgram(_shaderProgram);
    }

    for (auto &&shader: _shaders)
    {
        if (shader.first) {
            glDeleteShader(shader.first);
        }
    }
}

void ShaderProgram::use() {
    glUseProgram(_shaderProgram);
}

// /!\ Don't call each frame, it's very expensive
GLuint ShaderProgram::getUniformLocation(const char* location) {
    return glGetUniformLocation(_shaderProgram, location);
}

} // API
} // Graphics
