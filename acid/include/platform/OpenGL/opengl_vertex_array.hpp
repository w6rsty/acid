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

    virtual void SetVertexBuffer(const Ref<VertexBuffer>& buffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

    virtual const Ref<VertexBuffer>& GetVertexBuffer() const override { return vertexBuffer_; }
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer_; }

    virtual uint32_t GetIndexCount() const override { return indexBuffer_->GetCount(); }
private:
    uint32_t rendererID_;

    Ref<VertexBuffer> vertexBuffer_;
    Ref<IndexBuffer> indexBuffer_;
};

} // namespace acid