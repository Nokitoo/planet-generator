#include <Graphics/API/ShaderProgram.hpp> // Graphics::API::ShaderProgram

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

void ShaderProgram::use() const {
    glUseProgram(_shaderProgram);
}

// /!\ Don't call each frame, it's very expensive
GLuint ShaderProgram::getUniformLocation(const char* location) const {
    return glGetUniformLocation(_shaderProgram, location);
}

} // Namespace API
} // Namespace Graphics
