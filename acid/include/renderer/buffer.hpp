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
    VertexAttributeDataType Type;
    std::string Name;
    size_t Count;
    size_t Size;
    size_t Offset;
    bool Normalized;

    VertexBufferElement(VertexAttributeDataType type, const std::string& name, size_t count = 1, bool normalize = false)
    : Type(type), Name(name),Count(count), Size(VertexDataTypeSize(type) * count), Offset(0), Normalized(normalize)
    {}
};

class VertexBufferLayout
{
public:
    enum class LayoutType
    {
        Interleave,
        Batch
    };

    virtual LayoutType GetType() const = 0;
    virtual uint32_t GetStride() const = 0;

    virtual std::vector<VertexBufferElement>::iterator begin() = 0;
    virtual std::vector<VertexBufferElement>::iterator end() = 0;
    virtual std::vector<VertexBufferElement>::const_iterator begin() const = 0;
    virtual std::vector<VertexBufferElement>::const_iterator end() const = 0;
};


class VertexBufferLayoutInterleave final : public VertexBufferLayout
{
public:
    VertexBufferLayoutInterleave() = default;
    VertexBufferLayoutInterleave(const std::vector<VertexBufferElement>& elements)
    : elements_(elements)
    { CalculateStride(); }
    VertexBufferLayoutInterleave(const std::initializer_list<VertexBufferElement>& elements)
    : elements_(elements)
    { CalculateStride(); }

    virtual LayoutType GetType() const override { return LayoutType::Interleave; }
    virtual uint32_t GetStride() const override { return stride_; }

    virtual std::vector<VertexBufferElement>::iterator begin() override { return elements_.begin(); }
    virtual std::vector<VertexBufferElement>::iterator end() override { return elements_.end(); }
    virtual std::vector<VertexBufferElement>::const_iterator begin() const override { return elements_.begin(); }
    virtual std::vector<VertexBufferElement>::const_iterator end() const override { return elements_.end(); }
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

class VertexBufferLayoutBatch final : public VertexBufferLayout
{
public:
    VertexBufferLayoutBatch() = default;
    VertexBufferLayoutBatch(const std::vector<VertexBufferElement>& elements)
    : elements_(elements)
    { CalculateStride(); };
    VertexBufferLayoutBatch(const std::initializer_list<VertexBufferElement>& elements)
    : elements_(elements)
    { CalculateStride(); };

    virtual LayoutType GetType() const override { return LayoutType::Batch; }
    virtual uint32_t GetStride() const override { return stride_; }

    virtual std::vector<VertexBufferElement>::iterator begin() override { return elements_.begin(); }
    virtual std::vector<VertexBufferElement>::iterator end() override { return elements_.end(); }
    virtual std::vector<VertexBufferElement>::const_iterator begin() const override { return elements_.begin(); }
    virtual std::vector<VertexBufferElement>::const_iterator end() const override { return elements_.end(); }
private:
    void CalculateStride()
    {
        stride_ = 0;
        for (VertexBufferElement& element : elements_)
        {
            element.Offset = stride_;
            stride_ += element.Size;
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
    virtual void SetData(void* data, size_t size, size_t offset = 0) = 0;
    virtual void SetLayout(const Ref<VertexBufferLayout>& layout) = 0;
    virtual Ref<VertexBufferLayout>& GetLayout() = 0;

    static Ref<VertexBuffer> Create(size_t size);
    static Ref<VertexBuffer> Create(void* vertices, size_t size);
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(void* data, size_t size, size_t offset = 0) = 0;

    virtual uint32_t GetCount() const = 0;

    static Ref<IndexBuffer> Create(void* data, size_t size);
    static Ref<IndexBuffer> Create(size_t size);
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
    virtual uint32_t GetColorAttachmentID() const = 0;
    virtual uint32_t GetDepthAttachmentID() const = 0;

    static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
};

class UniformBuffer
{
public:
    virtual ~UniformBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const void* data, size_t size, uint32_t offset = 0) = 0;

    static Ref<UniformBuffer> Create(size_t size, uint32_t binding);
    static Ref<UniformBuffer> Create(size_t size, uint32_t binding, uint32_t offset);
};

} // namespace acid
