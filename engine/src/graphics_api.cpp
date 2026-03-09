#include "graphics_api.h"

#include "shader_program.h"

namespace engine
{

std::shared_ptr<ShaderProgram> GraphicsApi::CreateShaderProgram(const std::string& vertexSource,
                                                                const std::string& fragmentSource)
{
    auto vertexShader     = glCreateShader(GL_VERTEX_SHADER);
    auto vertexShaderCStr = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::array<char, 512> infoLog;
        glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
        return nullptr;
    }

    auto fragmentShader     = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShaderCStr = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::array<char, 512> infoLog;
        glGetShaderInfoLog(fragmentShader, infoLog.size(), nullptr, infoLog.data());
        return nullptr;
    }

    auto shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShader);
    glAttachShader(shaderProgramId, fragmentShader);
    glLinkProgram(shaderProgramId);

    if (!success)
    {
        std::array<char, 512> infoLog;
        glGetProgramInfoLog(shaderProgramId, infoLog.size(), nullptr, infoLog.data());
        return nullptr;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return std::make_shared<ShaderProgram>(shaderProgramId);
}

void GraphicsApi::BindShaderProgram(ShaderProgram* program) { program->Bind(); }

} // namespace engine