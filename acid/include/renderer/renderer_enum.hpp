#pragma once

#include <cstdint>

namespace acid
{

enum class VertexDataType
{
    None = 0,
    Bool,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
};

static uint32_t VertexDataTypeSize(VertexDataType type)
{
    switch (type) { 
        case VertexDataType::None:      return 0;      
        case VertexDataType::Bool:      return 4;
        case VertexDataType::Int:       return 4;
        case VertexDataType::Int2:      return 4 * 2;
        case VertexDataType::Int3:      return 4 * 3;
        case VertexDataType::Int4:      return 4 * 4;
        case VertexDataType::Float:     return 4;
        case VertexDataType::Float2:    return 4 * 2;
        case VertexDataType::Float3:    return 4 * 3;
        case VertexDataType::Float4:    return 4 * 4;
        case VertexDataType::Mat3:      return 4 * 3 * 3;
        case VertexDataType::Mat4:      return 4 * 4 * 4;
    }
}

static uint32_t VertexDataTypeCount(VertexDataType type)
{
    switch (type) { 
        case VertexDataType::None:      return 0;
        case VertexDataType::Bool:      return 1;
        case VertexDataType::Int:       return 1;
        case VertexDataType::Int2:      return 2;
        case VertexDataType::Int3:      return 3;
        case VertexDataType::Int4:      return 4;
        case VertexDataType::Float:     return 1;
        case VertexDataType::Float2:    return 2;
        case VertexDataType::Float3:    return 3;
        case VertexDataType::Float4:    return 4;
        case VertexDataType::Mat3:      return 3 * 3;
        case VertexDataType::Mat4:      return 4 * 4;
    }
}

} // namespace acid