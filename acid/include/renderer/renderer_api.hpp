#pragma once

#include "core/base.hpp"
#include "renderer/vertex_array.hpp"

#include "glm/glm.hpp"

namespace acid
{

class RendererAPI
{
public:
    enum class API
    {
        None = 0,
        OpenGL = 1,
    };

    static API GetAPI() { return s_API; }

    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void DrawWireFrame(bool enable) = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;
    virtual void DrawIndexedArray(const Ref<VertexArray>& vertedxArray, uint32_t indexCount, uint32_t count) = 0;
private:
    static API s_API;
};

} // namespace acid