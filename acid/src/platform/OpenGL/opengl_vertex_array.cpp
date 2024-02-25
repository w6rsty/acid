#include "platform/OpenGL/opengl_vertex_array.hpp"

#include "core/assert.hpp"

#include "glad/glad.h"
#include "renderer/buffer.hpp"
#include "renderer/renderer_enum.hpp"

namespace acid
{

static GLenum VertexDataTypeToGLenum(VertexDataType type)
{
    switch(type)
    {
        case VertexDataType::None: {
            AC_ASSERT(false);
            return 0;
        }
        case VertexDataType::Bool:      return GL_BOOL;
        case VertexDataType::Int:       return GL_INT;
        case VertexDataType::Int2:      return GL_INT;
        case VertexDataType::Int3:      return GL_INT;
        case VertexDataType::Int4:      return GL_INT;
        case VertexDataType::Float:     return GL_FLOAT;
        case VertexDataType::Float2:    return GL_FLOAT;
        case VertexDataType::Float3:    return GL_FLOAT;
        case VertexDataType::Float4:    return GL_FLOAT;
        case VertexDataType::Mat3:      return GL_FLOAT;
        case VertexDataType::Mat4:      return GL_FLOAT;
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

void OpenGLVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& buffer)
{
    glBindVertexArray(rendererID_);
    const VertexBufferLayout layout = buffer->GetLayout();
    int index = 0;
    for (const VertexBufferElement& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            VertexDataTypeCount(element.Type),
            VertexDataTypeToGLenum(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<void*>(element.Offset)
        );
        index++;
    }

    vertexBuffer_ = buffer;   
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
{
    indexBuffer_ = buffer;
}

} // namespace acid