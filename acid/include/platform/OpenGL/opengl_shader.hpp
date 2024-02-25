#pragma once

#include "renderer/shader.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace acid
{

class OpenGLShader final : public Shader
{
public:
    OpenGLShader(const std::string& filename);
    OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~OpenGLShader() override;
    
    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetUniformInt(const std::string& name, const int value) override;
    virtual void SetUniformFloat(const std::string& name, const float value) override;
    virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
    virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
    virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;
    virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
private:
    std::string ReadFile(const std::string& filename);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& content);
    void CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources);

    virtual void UploadUniformInt(const std::string& name, const int value) override;
    virtual void UploadUniformFloat(const std::string& name, const float value) override;
    virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) override;
    virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) override;
    virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) override;
    virtual void UploadUniformMat3(const std::string& name, const glm::mat3& value) override;
    virtual void UploadUniformMat4(const std::string& name, const glm::mat4& value) override;
private:
    uint32_t rendererID_;
};

} // namespace acid