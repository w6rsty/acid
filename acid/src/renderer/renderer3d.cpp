#include "renderer/renderer3d.hpp"

#include "core/log.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "renderer/camera/scene_camera.hpp"
#include "renderer/renderer_enum.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/renderer_command.hpp"

#include <array>

namespace acid
{

struct Vertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
    float TexIndex;
};

struct RendererData
{
    static const uint32_t MaxVoxels = 1024;
    static const uint32_t MaxVertices = MaxVoxels * 8;
    static const uint32_t MaxIndices = MaxVoxels * 36;
    static const uint32_t MaxTextureSlots = 16;

    Ref<VertexArray> BatchVA = nullptr;
    Ref<Shader> BatchShader = nullptr;

    Vertex* BatchVertexBufferBase = nullptr;
    Vertex* BatchVertexBufferPtr = nullptr;

    uint32_t IndexCount = 0;

    RendererStats Stats;

    std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1;

    const glm::vec3 CubeVertices[8] = {
        { -0.125f, -0.125f, -0.125f },
        {  0.125f, -0.125f, -0.125f },
        {  0.125f,  0.125f, -0.125f },
        { -0.125f,  0.125f, -0.125f },
        { -0.125f, -0.125f,  0.125f },
        {  0.125f, -0.125f,  0.125f },
        {  0.125f,  0.125f,  0.125f },
        { -0.125f,  0.125f,  0.125f }
    };

    const glm::vec2 TexCoords[8] {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },

        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
    };

    Ref<SceneCamera> Camera = nullptr;
};

static RendererData sData;

void Renderer3D::Init()
{
    sData.Stats = {};
    sData.BatchVA = VertexArray::Create();
    sData.BatchVA->Bind();
    
    auto vertexBuffer = VertexBuffer::Create(RendererData::MaxVertices * sizeof(Vertex));
    vertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" },
        { VertexDataType::Float4, "a_Color" },
        { VertexDataType::Float2, "a_TexCoord" },
        { VertexDataType::Float3, "a_Normal" },
        { VertexDataType::Float, "a_TexIndex"},
    });
    sData.BatchVA->SetVertexBuffer(vertexBuffer);
    sData.BatchVertexBufferBase = new Vertex[sData.MaxVertices];
  
    uint32_t* indices = new uint32_t[RendererData::MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < RendererData::MaxIndices; i += 36)
    {
        // front
        indices[i +  0] = offset + 0;
        indices[i +  1] = offset + 1;
        indices[i +  2] = offset + 2;
        indices[i +  3] = offset + 2;
        indices[i +  4] = offset + 3;
        indices[i +  5] = offset + 0;
        // back
        indices[i +  6] = offset + 5;
        indices[i +  7] = offset + 4;
        indices[i +  8] = offset + 7;
        indices[i +  9] = offset + 7;
        indices[i + 10] = offset + 6;
        indices[i + 11] = offset + 5;
        // left
        indices[i + 12] = offset + 4;
        indices[i + 13] = offset + 0;
        indices[i + 14] = offset + 3;
        indices[i + 15] = offset + 3;
        indices[i + 16] = offset + 7;
        indices[i + 17] = offset + 4;
        // right
        indices[i + 18] = offset + 1;
        indices[i + 19] = offset + 5;
        indices[i + 20] = offset + 6;
        indices[i + 21] = offset + 6;
        indices[i + 22] = offset + 2;
        indices[i + 23] = offset + 1;
        // top
        indices[i + 24] = offset + 3;
        indices[i + 25] = offset + 2;
        indices[i + 26] = offset + 6;
        indices[i + 27] = offset + 6;
        indices[i + 28] = offset + 7;
        indices[i + 29] = offset + 3;
        // bottom
        indices[i + 30] = offset + 1;
        indices[i + 31] = offset + 0;
        indices[i + 32] = offset + 4;
        indices[i + 33] = offset + 4;
        indices[i + 34] = offset + 5;
        indices[i + 35] = offset + 1;

        offset += 8;
    }
    
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, RendererData::MaxIndices * sizeof(uint32_t));
    sData.BatchVA->SetIndexBuffer(indexBuffer);
    delete[] indices;

    sData.TextureSlots[0] = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    sData.TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));
    sData.TextureSlots[0]->Bind(0);

    int samplers[sData.MaxTextureSlots];
    for (uint32_t i = 0; i < sData.MaxTextureSlots; i++)
    {
        samplers[i] = i;
    }

    sData.BatchShader = Shader::Create("assets/shaders/voxel.shader");
    sData.BatchShader->Bind();
    sData.BatchShader->SetUniformIntArray("u_Textures", samplers, sData.MaxTextureSlots);
}

void Renderer3D::Shutdown()
{
    delete[] sData.BatchVertexBufferBase;
}

void Renderer3D::StartBatch()
{
    sData.BatchVertexBufferPtr = sData.BatchVertexBufferBase;
    sData.IndexCount = 0;
}

void Renderer3D::NextBatch()
{
    Flush();
    StartBatch();
}

void Renderer3D::Flush()
{
    if (sData.IndexCount == 0) return;

    for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
    {
        sData.TextureSlots[i]->Bind(i);
    }

    uint32_t dataSize = sData.BatchVertexBufferPtr - sData.BatchVertexBufferBase;
    sData.BatchVA->GetVertexBuffer()->SetData(sData.BatchVertexBufferBase, dataSize * sizeof(Vertex));
    RendererCommand::DrawIndexed(sData.BatchVA, sData.IndexCount);
    sData.Stats.DrawCalls++;
}

void Renderer3D::BeginScene(const Ref<SceneCamera>& camera)
{
    sData.Stats.Reset();
    sData.BatchShader->Bind();
    sData.BatchShader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());
    sData.BatchShader->SetUniformMat4("u_View", camera->GetViewMatrix());

    sData.BatchShader->SetUniformFloat3("u_CamPos", camera->GetPosition());

    sData.BatchShader->SetUniformFloat3("u_Light.Position", { 0.0f, 3.0f, 0.0f });
    sData.BatchShader->SetUniformFloat3("u_Light.Direction", { -1.0f, -1.0f, 0.0f });
    sData.BatchShader->SetUniformFloat3("u_Light.Ambient", { 0.1f, 0.1f, 0.1f });
    sData.BatchShader->SetUniformFloat("u_Light.AmbientIntensity", 0.1f);
    sData.BatchShader->SetUniformFloat3("u_Light.Diffuse", { 0.4f, 0.4f, 0.4f });
    sData.BatchShader->SetUniformFloat("u_Light.DiffuseIntensity", 0.8f);
    sData.BatchShader->SetUniformFloat3("u_Light.Specular", { 0.5f, 0.5f, 0.5f });
    sData.BatchShader->SetUniformFloat("u_Light.SpecularIntensity", 0.5f);
    sData.BatchShader->SetUniformFloat("u_Light.Shininess", 4.0f);

    sData.Camera = camera;

    StartBatch();
}

void Renderer3D::EndScene()
{   
    Flush();

    sData.Camera = nullptr;
}

void Renderer3D::DrawVoxel(const glm::mat4& transform, const glm::vec4& color)
{
    if (sData.IndexCount >= RendererData::MaxIndices)
    {
        NextBatch();
    }

    for (uint32_t i = 0; i < 8; i++)
    {
        glm::vec3 Pos = transform * glm::vec4(sData.CubeVertices[i], 1.0);
        sData.BatchVertexBufferPtr->Position = Pos;
        sData.BatchVertexBufferPtr->Color = color;
        sData.BatchVertexBufferPtr->TexCoord = sData.TexCoords[i];
        sData.BatchVertexBufferPtr->Normal = Pos;
        sData.BatchVertexBufferPtr->TexIndex = 0.0f;
        sData.BatchVertexBufferPtr++;
    }

    sData.IndexCount += 36;

    sData.Stats.VertexCount += 8;
    sData.Stats.TriangleCount += 12;
    sData.Stats.VoxelCount++;
}

void Renderer3D::DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color)
{
    if (sData.IndexCount >= RendererData::MaxIndices)
    {
        NextBatch();
    }

    float textureIndex = 0.0f;
    for (int i = 1; i < sData.TextureSlotIndex; i++)
    {
        if (*sData.TextureSlots[i].get() == *texture.get())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        textureIndex = (float)sData.TextureSlotIndex;
        sData.TextureSlots[sData.TextureSlotIndex] = texture;
        sData.TextureSlotIndex++;
    }

    for (uint32_t i = 0; i < 8; i++)
    {
        glm::vec3 Pos = transform * glm::vec4(sData.CubeVertices[i], 1.0);
        sData.BatchVertexBufferPtr->Position = Pos;
        sData.BatchVertexBufferPtr->Color = color;
        sData.BatchVertexBufferPtr->TexCoord = sData.TexCoords[i];
        sData.BatchVertexBufferPtr->Normal = Pos;
        sData.BatchVertexBufferPtr->TexIndex = textureIndex;
        sData.BatchVertexBufferPtr++;
    }

    sData.IndexCount += 36;

    sData.Stats.VertexCount += 8;
    sData.Stats.TriangleCount += 12;
    sData.Stats.VoxelCount++;
}


const RendererStats& Renderer3D::GetStats()
{
    return sData.Stats;
}

} // namespace acid