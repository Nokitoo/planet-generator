#pragma once

#include <unordered_map> // std::unordered_map

#include <GL/glew.h> // GLenum, GLuint

namespace Graphics {
namespace API {

namespace Builder {
    class ShaderProgram;
}

class ShaderProgram {
    friend Builder::ShaderProgram;
public:
    ShaderProgram() = default;
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram& shaderProgram) = delete;
    ShaderProgram(ShaderProgram&& shaderProgram);

    ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;
    ShaderProgram& operator=(ShaderProgram&& shaderProgram);

    void use() const;

    GLuint getUniformLocation(const char* location) const;

private:
    GLuint _shaderProgram = 0;

    std::unordered_map<GLenum, GLuint> _shaders;
};

} // Namespace API
} // Namespace Graphics
