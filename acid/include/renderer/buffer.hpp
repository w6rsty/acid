#pragma once

#include "core/base.hpp"
#include "renderer/buffer.hpp"
#include "renderer/renderer_enum.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <initializer_list>

namespace acid
{

struct VertexBufferElement
{
    VertexDataType  Type;
    std::string     Name;
    uint32_t        Size;
    uint32_t        Offset;
    bool            Normalized;

    VertexBufferElement(VertexDataType type, const std::string& name, bool normalize = false)
    : Type(type), Name(name), Size(VertexDataTypeSize(type)), Offset(0), Normalized(normalize)
    {}
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() = default;
    VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
    : elements_(elements)
    {
        CalculateStride();
    }

    uint32_t GetStride() const { return stride_; }

    std::vector<VertexBufferElement>::iterator begin() { return elements_.begin(); }
    std::vector<VertexBufferElement>::iterator end() { return elements_.end(); }
    std::vector<VertexBufferElement>::const_iterator begin() const { return elements_.begin(); }
    std::vector<VertexBufferElement>::const_iterator end() const { return elements_.end(); }
private:
    void CalculateStride()
    {
        stride_ = 0;
        for (VertexBufferElement& elements : elements_)
        {
            elements.Offset = stride_;
            stride_ += elements.Size;
        }
    }
private:
    std::vector<VertexBufferElement> elements_;
    uint32_t stride_ = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;
    
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(float* data, size_t size) = 0;
    virtual void SetLayout(const VertexBufferLayout& layout) = 0;
    virtual VertexBufferLayout& GetLayout() = 0;

    static Ref<VertexBuffer> Create(size_t size);
    static Ref<VertexBuffer> Create(float* vertices, size_t size);
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Ref<IndexBuffer> Create(uint32_t* data, size_t count);
};

struct FrameBufferSpecification
{
    uint32_t Width, Height;
    uint32_t Samples = 1;
    bool SwapChainTarget = false;
};

class FrameBuffer
{
public:
    virtual ~FrameBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
};

} // namespace acid
