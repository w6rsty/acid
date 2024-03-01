#pragma once

#include "core/base.hpp"

#include "glm/glm.hpp"

namespace acid
{

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Ref<Shader> Create(const std::string& filename);
    static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);

    virtual void SetUniformInt(const std::string& name, const int value) = 0;
    virtual void SetUniformIntArray(const std::string& name, int* values, uint32_t count) = 0;
    virtual void SetUniformFloat(const std::string& name, const float value) = 0;
    virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
protected:
    virtual void UploadUniformInt(const std::string& name, const int value) = 0;
    virtual void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) = 0;
    virtual void UploadUniformFloat(const std::string& name, const float value) = 0;
    virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
    virtual void UploadUniformMat3(const std::string& name, const glm::mat3& value) = 0;
    virtual void UploadUniformMat4(const std::string& name, const glm::mat4& value) = 0;
};

} // namespace acid