#pragma once

#include "renderer/buffer.hpp"

namespace acid
{

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(size_t size);
    OpenGLVertexBuffer(void* vertices, size_t size);
    virtual ~OpenGLVertexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetData(void* data, size_t size, size_t offset) override;
    virtual void SetLayout(const VertexBufferLayout& layout) override { layout_ = layout; }
    virtual VertexBufferLayout& GetLayout() override { return layout_; }
private:
    uint32_t rendererID_;
    VertexBufferLayout layout_;
};

class OpenGLIndexBuffer final : public IndexBuffer
{
public:
    OpenGLIndexBuffer(size_t size);
    OpenGLIndexBuffer(void* data, size_t size);
    virtual ~OpenGLIndexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetData(void* data, size_t size, size_t offset) override;

    virtual uint32_t GetCount() const override { return size_ / sizeof(uint32_t); }
private:
    uint32_t rendererID_;
    uint32_t size_;
};

class OpenGLFrameBuffer final : public FrameBuffer
{
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual uint32_t GetColorAttachmentID() const override { return colorAttachment_; }
    virtual uint32_t GetDepthAttachmentID() const override { return depthAttachment_; }
    
private:
    uint32_t rendererID_;
    FrameBufferSpecification spec_;
    uint32_t colorAttachment_;
    uint32_t depthAttachment_;
};
 
} // namespace acid