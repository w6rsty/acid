#include "platform/OpenGL/opengl_vertex_array.hpp"

#include "core/assert.hpp"

#include "core/log.hpp"
#include "glad/glad.h"
#include "renderer/buffer.hpp"
#include "renderer/renderer_enum.hpp"

namespace acid
{

static GLenum VertexDataTypeToGLenum(VertexAttributeDataType type)
{
    switch(type)
    {
        case VertexAttributeDataType::Bool:      return GL_BOOL;
        case VertexAttributeDataType::Int:       return GL_INT;
        case VertexAttributeDataType::Int2:      return GL_INT;
        case VertexAttributeDataType::Int3:      return GL_INT;
        case VertexAttributeDataType::Int4:      return GL_INT;
        case VertexAttributeDataType::Float:     return GL_FLOAT;
        case VertexAttributeDataType::Float2:    return GL_FLOAT;
        case VertexAttributeDataType::Float3:    return GL_FLOAT;
        case VertexAttributeDataType::Float4:    return GL_FLOAT;
        case VertexAttributeDataType::Mat3:      return GL_FLOAT;
        case VertexAttributeDataType::Mat4:      return GL_FLOAT;
        case VertexAttributeDataType::None:      return GL_NONE;
    }
}


OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &rendererID_);
    glBindVertexArray(rendererID_);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &rendererID_);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(rendererID_);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::ProcessVertexBufferImpl(Ref<VertexBufferLayoutInterleave> layout)
{
    int index = 0;
    for (const VertexBufferElement& element : *layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            VertexDataTypeCount(element.Type),
            VertexDataTypeToGLenum(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout->GetStride(),
            reinterpret_cast<void*>(element.Offset)
        );
        index++;
    }
}
void OpenGLVertexArray::ProcessVertexBufferImpl(Ref<VertexBufferLayoutBatch> layout)
{
    int index = 0;
    for (const VertexBufferElement& element : *layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            VertexDataTypeCount(element.Type),
            VertexDataTypeToGLenum(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            VertexDataTypeSize(element.Type),
            reinterpret_cast<void*>(element.Offset)
        );
        index++;
    }
}

void OpenGLVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& buffer)
{
    glBindVertexArray(rendererID_);
    auto layout = buffer->GetLayout();
    switch (layout->GetType())
    {
        case VertexBufferLayout::LayoutType::Interleave:
            ProcessVertexBufferImpl(std::static_pointer_cast<VertexBufferLayoutInterleave>(layout));
            break;
        case VertexBufferLayout::LayoutType::Batch:
            ProcessVertexBufferImpl(std::static_pointer_cast<VertexBufferLayoutBatch>(layout));
            break;
    }


    vertexBuffer_ = buffer;   
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
{
    indexBuffer_ = buffer;
}

} // namespace acid