#pragma once

#include "core/assert.hpp"

#include <cstdint>

#include "glad/glad.h"
#include "tiny_gltf.h"

namespace acid
{

enum class VertexAttributeDataType
{
    None = 0,
    Bool,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
};

static uint32_t VertexDataTypeSize(VertexAttributeDataType type)
{
    switch (type) {    
        case VertexAttributeDataType::Bool:      return 4;
        case VertexAttributeDataType::Int:       return 4;
        case VertexAttributeDataType::Int2:      return 4 * 2;
        case VertexAttributeDataType::Int3:      return 4 * 3;
        case VertexAttributeDataType::Int4:      return 4 * 4;
        case VertexAttributeDataType::Float:     return 4;
        case VertexAttributeDataType::Float2:    return 4 * 2;
        case VertexAttributeDataType::Float3:    return 4 * 3;
        case VertexAttributeDataType::Float4:    return 4 * 4;
        case VertexAttributeDataType::Mat3:      return 4 * 3 * 3;
        case VertexAttributeDataType::Mat4:      return 4 * 4 * 4;
        case VertexAttributeDataType::None:      return 0;
    }
}

static uint32_t VertexDataTypeCount(VertexAttributeDataType type)
{
    switch (type) { 
        case VertexAttributeDataType::Bool:      return 1;
        case VertexAttributeDataType::Int:       return 1;
        case VertexAttributeDataType::Int2:      return 2;
        case VertexAttributeDataType::Int3:      return 3;
        case VertexAttributeDataType::Int4:      return 4;
        case VertexAttributeDataType::Float:     return 1;
        case VertexAttributeDataType::Float2:    return 2;
        case VertexAttributeDataType::Float3:    return 3;
        case VertexAttributeDataType::Float4:    return 4;
        case VertexAttributeDataType::Mat3:      return 3 * 3;
        case VertexAttributeDataType::Mat4:      return 4 * 4;
        case VertexAttributeDataType::None:      return 0;
    }
}

enum class AC_TEXTURE_WARP_MODE
{
    REPEAT = 0,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
};

static GLenum AC_TEXTURE_WARP_MODE_TO_GL(AC_TEXTURE_WARP_MODE mode)
{
    switch (mode) {
        case AC_TEXTURE_WARP_MODE::REPEAT: return GL_REPEAT;
        case AC_TEXTURE_WARP_MODE::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
        case AC_TEXTURE_WARP_MODE::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
        case AC_TEXTURE_WARP_MODE::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
    }
}


/// =========================
/// TinyGLTF type conversions
/// =========================

static VertexAttributeDataType TypeToVertexAttributeDataType(const int type)
{
    switch (type)
    {
        case TINYGLTF_TYPE_SCALAR: return VertexAttributeDataType::Float;
        case TINYGLTF_TYPE_VEC2: return VertexAttributeDataType::Float2;
        case TINYGLTF_TYPE_VEC3: return VertexAttributeDataType::Float3;
        case TINYGLTF_TYPE_VEC4: return VertexAttributeDataType::Float4;
        case TINYGLTF_TYPE_MAT3: return VertexAttributeDataType::Mat3;
        case TINYGLTF_TYPE_MAT4: return VertexAttributeDataType::Mat4;
        default: 
        {
            AC_LOG_ERROR("Unknown type: {}", type);
            return VertexAttributeDataType::None;
        }
    }
}

static GLenum ComponentTypeToGLenum(const int type)
{
    switch (type)
    {
        case TINYGLTF_COMPONENT_TYPE_BYTE: return GL_BYTE;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
        case TINYGLTF_COMPONENT_TYPE_SHORT: return GL_SHORT;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
        case TINYGLTF_COMPONENT_TYPE_INT: return GL_INT;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: return GL_UNSIGNED_INT;
        case TINYGLTF_COMPONENT_TYPE_FLOAT: return GL_FLOAT;
        case TINYGLTF_COMPONENT_TYPE_DOUBLE: return GL_DOUBLE;
        default: return GL_NONE;
    }
}

static GLenum TargetToGLenum(const int target)
{
    switch (target)
    {
        case TINYGLTF_TARGET_ARRAY_BUFFER: return GL_ARRAY_BUFFER;
        case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER: return GL_ELEMENT_ARRAY_BUFFER;
        default: return GL_NONE;
    }
}

} // namespace acid