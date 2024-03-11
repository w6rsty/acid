#include "geometry/model.hpp"

#include "core/assert.hpp"
#include "core/log.hpp"
#include "renderer/buffer.hpp"
#include "renderer/renderer_enum.hpp"
#include "renderer/vertex_array.hpp"

#include <string_view>

namespace acid
{

GltfModel::GltfModel(std::string_view filename)
{
    Load(filename);
}

void GltfModel::Load(std::string_view filename)
{
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model_, &err, &warn, "assets/models/cube.gltf");

    if (!warn.empty())
    {
        AC_LOG_ERROR(warn);
    }

    modelVAO_ = VertexArray::Create();
    modelVAO_->Bind();

    size_t vboSize = 0;
    size_t iboSize = 0;

    void* iboData = nullptr;
    std::vector<VertexBufferElement> layoutElements;

    for (const auto& accessor : model_.accessors)
    {
        auto info = HandleAccessor(accessor);
        if (info.BufferType == GL_ARRAY_BUFFER)
        {
            vboSize += info.ByteLength;
            layoutElements.push_back({info.Type, "", info.Count});
        }
        else if (info.BufferType == GL_ELEMENT_ARRAY_BUFFER)
        {
            iboSize += info.ByteLength;
            iboData = info.Data;
        }
    }

    Ref<VertexBuffer> vbo = VertexBuffer::Create(vboSize);
    vbo->SetLayout(CreateRef<VertexBufferLayoutBatch>(layoutElements));
    uint32_t offset = 0;
    for (const auto& accessor : model_.accessors)
    {
        auto info = HandleAccessor(accessor);
        if (info.BufferType == GL_ARRAY_BUFFER)
        {
            void* dataPtr = info.Data;
            vbo->SetData(dataPtr, info.ByteLength, offset);
            offset += info.ByteLength;
        }
    }
    modelVAO_->SetVertexBuffer(vbo);
    Ref<IndexBuffer> ibo = IndexBuffer::Create(iboData, iboSize);
    modelVAO_->SetIndexBuffer(ibo);
}



GltfAttributeBatch  GltfModel::HandleAccessor(const tinygltf::Accessor& accessor)
{
    GltfAttributeBatch batch;

    auto& bufferView = model_.bufferViews[accessor.bufferView];
    auto& buffer = model_.buffers[bufferView.buffer];

    batch.Count = accessor.count;
    batch.BufferType = TargetToGLenum(bufferView.target);
    batch.Type = TypeToVertexAttributeDataType(accessor.type);
    batch.ComponentType = ComponentTypeToGLenum(accessor.componentType);
    batch.ByteOffset = bufferView.byteOffset;
    batch.ByteLength = bufferView.byteLength;
    batch.Data = buffer.data.data() + bufferView.byteOffset;

    return batch;
}


} // namespace acid