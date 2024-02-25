#pragma once

#include "renderer/buffer.hpp"

namespace acid
{

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(size_t size);
    OpenGLVertexBuffer(float* vertices, size_t size);
    virtual ~OpenGLVertexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetData(float* data, size_t size) override;
    virtual void SetLayout(const VertexBufferLayout& layout) override { layout_ = layout; }
    virtual VertexBufferLayout& GetLayout() override { return layout_; }
private:
    uint32_t rendererID_;
    VertexBufferLayout layout_;
};

class OpenGLIndexBuffer final : public IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t* data, size_t count);
    virtual ~OpenGLIndexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t GetCount() const override { return count_; }
private:
    uint32_t rendererID_;
    uint32_t count_;
};

class OpenGLFrameBuffer final : public FrameBuffer
{
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
    uint32_t rendererID_;
};
 
} // namespace acid