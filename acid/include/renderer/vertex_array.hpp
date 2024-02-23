#pragma once

#include "core/base.hpp"
#include "renderer/buffer.hpp"

namespace acid
{

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

    virtual uint32_t GetIndexCount() const = 0;

    static Ref<VertexArray> Create();
};

} // namespace acid