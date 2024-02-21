#pragma once

#include "core/base.hpp"
#include "renderer/vertex_array.hpp"

namespace acid
{

class OpenGLVertexArray final : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetVertexBuffer(const Ref<VertexBuffer>& buffer) override { vertexBuffer_ = buffer; }
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override { indexBuffer_ = buffer; }
private:
    uint32_t rendererID_;

    Ref<VertexBuffer> vertexBuffer_;
    Ref<IndexBuffer> indexBuffer_;
};

} // namespace acid