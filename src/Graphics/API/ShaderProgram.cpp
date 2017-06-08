#include <Graphics/API/ShaderProgram.hpp> // Graphics::API::ShaderProgram

namespace Graphics {
namespace API {

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) {
    _shaderProgram = shaderProgram._shaderProgram;
    _shaders = shaderProgram._shaders;

    shaderProgram._shaderProgram = 0;
    shaderProgram._shaders.clear();
    shaderProgram._locations.clear();
}


ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) {
    destroy();

    _shaderProgram = shaderProgram._shaderProgram;
    _shaders = shaderProgram._shaders;

    shaderProgram._shaderProgram = 0;
    shaderProgram._shaders.clear();
    shaderProgram._locations.clear();

    return *this;
}

ShaderProgram::~ShaderProgram() {
    destroy();
}

void ShaderProgram::use() const {
    glUseProgram(_shaderProgram);
}

// /!\ Don't call each frame, it's very expensive
GLuint ShaderProgram::getUniformLocation(const std::string& locationName) {
    auto location = _locations.find(locationName);
    if (location != _locations.end()) {
        return location->second;
    }

    _locations[locationName] = glGetUniformLocation(_shaderProgram, locationName.c_str());
    return _locations.at(locationName);
}

GLuint ShaderProgram::getUniformLocation(const std::string& locationName) const {
    return _locations.at(locationName);
}

void ShaderProgram::destroy() {
    if (_shaderProgram) {
        glDeleteProgram(_shaderProgram);
        _shaderProgram = 0;
    }

    for (auto &&shader: _shaders)
    {
        if (shader.first) {
            glDeleteShader(shader.first);
        }
    }

    _shaders.clear();
    _locations.clear();
}

} // Namespace API
} // Namespace Graphics
