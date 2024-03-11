#pragma once

#include "core/base.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/renderer_enum.hpp"

#include "tiny_gltf.h"

#include <string_view>

namespace acid
{

struct GltfAttributeBatch
{
    void* Data;
    size_t Count;
    GLenum BufferType; // GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
    VertexAttributeDataType Type; // engine build in vertex data type
    GLenum ComponentType; // to set type in glVertexAttribPointer
    size_t ByteOffset;
    size_t ByteLength;
};

class GltfModel
{
public:
    GltfModel(std::string_view filename);

    Ref<VertexArray> GetVertexArray() const { return modelVAO_; }
private:
    void Load(std::string_view filename);
    GltfAttributeBatch HandleAccessor(const tinygltf::Accessor& accessor);
private:
    tinygltf::Model model_;
    Ref<VertexArray> modelVAO_;
};

} // namespace acid
