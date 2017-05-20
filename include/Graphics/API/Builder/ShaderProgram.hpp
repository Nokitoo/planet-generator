#pragma once

#include <fstream> // std::ifstream
#include <string> // std::string
#include <unordered_map> // std::unordered_map

#include <Graphics/API/ShaderProgram.hpp>

namespace Graphics {
namespace API {
namespace Builder {

class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram& shaderProgram) = delete;
    ShaderProgram(ShaderProgram&& shaderProgram) = delete;

    ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;
    ShaderProgram& operator=(ShaderProgram&& shaderProgram) = delete;

    bool build(API::ShaderProgram& shaderProgram);

    bool setShader(GLenum shaderType, const std::string& shaderFileName);

private:
    bool checkShaderStatus(GLuint shader, GLenum statusName);
    bool checkProgramStatus(GLuint shaderProgram, GLenum statusName);

private:
    std::unordered_map<GLenum, std::ifstream> _shadersFiles;
};

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
