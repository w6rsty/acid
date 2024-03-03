#include "renderer/renderer3d.hpp"

#include "core/log.hpp"
#include "geometry/cuboid.hpp"
#include "renderer/buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/renderer_enum.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/renderer_command.hpp"
#include "renderer/camera/scene_camera.hpp"

#include "glm/ext/matrix_transform.hpp"

#include <array>
#include <functional>

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
    static const uint32_t MaxVertices = MaxVoxels * 24;
    static const uint32_t MaxIndices = MaxVoxels * 36;
    static const uint32_t MaxTextureSlots = 16;

    Ref<VertexArray> BatchVA = nullptr;
    Ref<Shader> BatchShader = nullptr;
    Ref<UniformBuffer> UniformBuffer = nullptr;
    std::function<void()> SetUniformData = [&]() {
        BatchShader->BindUniformBlock("Matrices", 0);
        UniformBuffer->SetData(&Camera->GetProjectionMatrix(), sizeof(glm::mat4));
        UniformBuffer->SetData(&Camera->GetViewMatrix(), sizeof(glm::mat4), sizeof(glm::mat4));
    };

    Vertex* BatchVertexBufferBase = nullptr;
    Vertex* BatchVertexBufferPtr = nullptr;

    uint32_t IndexCount = 0;

    RendererStats Stats;

    std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
    // reserve for blank, bulb, flashlight
    uint32_t TextureSlotIndex = 3;

    float* Gamma = nullptr;

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
    for (uint32_t i = 0; i < RendererData::MaxIndices; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }
    
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, RendererData::MaxIndices * sizeof(uint32_t));
    sData.BatchVA->SetIndexBuffer(indexBuffer);
    delete[] indices;

    sData.UniformBuffer = UniformBuffer::Create(2 * sizeof(glm::mat4), 0);

    sData.TextureSlots[0] = Texture2D::Create(1, 1);
    uint32_t blankTex = 0xffffffff;
    sData.TextureSlots[0]->SetData(&blankTex, sizeof(uint32_t));
    sData.TextureSlots[0]->Bind(0);

    sData.TextureSlots[1] = Texture2D::Create("assets/textures/bulb.png");
    sData.TextureSlots[1]->Bind(1);

    sData.TextureSlots[2] = Texture2D::Create("assets/textures/flashlight.png");
    sData.TextureSlots[2]->Bind(2);

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
    if (!sData.Camera) {
        sData.Camera = camera;
        sData.SetUniformData();
    }

    sData.Stats.Reset();
    sData.BatchShader->Bind();

    sData.BatchShader->SetUniformFloat3("u_CamPos", camera->GetPosition());
    if (sData.Gamma)
    {   
        sData.BatchShader->SetUniformFloat("u_Gamma", *sData.Gamma);
    }

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

    for (uint32_t i = 0; i < 24; i++)
    {
        glm::vec3 Pos = transform * glm::vec4(Cuboid::Vertices[i], 1.0);
        sData.BatchVertexBufferPtr->Position = Pos;
        sData.BatchVertexBufferPtr->Color = color;
        sData.BatchVertexBufferPtr->TexCoord = Cuboid::TexCoords[i];
        sData.BatchVertexBufferPtr->Normal = Cuboid::Normals[i / 4];
        sData.BatchVertexBufferPtr->TexIndex = 0.0f;
        sData.BatchVertexBufferPtr++;
    }

    sData.IndexCount += 36;

    sData.Stats.VertexCount += 24;
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

    for (uint32_t i = 0; i < 24; i++)
    {
        glm::vec3 Pos = transform * glm::vec4(Cuboid::Vertices[i], 1.0);
        sData.BatchVertexBufferPtr->Position = Pos;
        sData.BatchVertexBufferPtr->Color = color;
        sData.BatchVertexBufferPtr->TexCoord = Cuboid::TexCoords[i];
        sData.BatchVertexBufferPtr->Normal = Cuboid::Normals[i / 4];
        sData.BatchVertexBufferPtr->TexIndex = textureIndex;
        sData.BatchVertexBufferPtr++;
    }

    sData.IndexCount += 36;

    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
    sData.Stats.VoxelCount++;
}

void Renderer3D::DrawPointLight(const glm::vec3& position, PointLight& light, uint32_t index)
{
    if (sData.IndexCount >= RendererData::MaxIndices)
    {
        NextBatch();
    }

    light.Position = position;
    SetPointLightUniforms(light, index);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

    for (uint32_t i = 0; i < 24; i++)
    {
        glm::vec3 Pos = transform * glm::vec4(Cuboid::Vertices[i], 1.0);
        sData.BatchVertexBufferPtr->Position = Pos;
        sData.BatchVertexBufferPtr->Color = glm::vec4(1.0f);
        sData.BatchVertexBufferPtr->TexCoord = Cuboid::TexCoords[i];
        sData.BatchVertexBufferPtr->Normal = Cuboid::Normals[i / 4];
        sData.BatchVertexBufferPtr->TexIndex = 1.0f;
        sData.BatchVertexBufferPtr++;
    }

    sData.IndexCount += 36;

    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
    sData.Stats.VoxelCount++;
}

void Renderer3D::DrawSpotLight(const glm::vec3 &position, SpotLight &Light)
{
    if (sData.IndexCount >= RendererData::MaxIndices)
    {
        NextBatch();
    }

    Light.Position = position;
    SetSpotLightUniforms(Light);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

    for (uint32_t i = 0; i < 24; i++)
    {
        glm::vec3 Pos = transform * glm::vec4(Cuboid::Vertices[i], 1.0);
        sData.BatchVertexBufferPtr->Position = Pos;
        sData.BatchVertexBufferPtr->Color = glm::vec4(1.0f);
        sData.BatchVertexBufferPtr->TexCoord = Cuboid::TexCoords[i];
        sData.BatchVertexBufferPtr->Normal = Cuboid::Normals[i / 4];
        sData.BatchVertexBufferPtr->TexIndex = 2.0f;
        sData.BatchVertexBufferPtr++;
    }

    sData.IndexCount += 36;

    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
    sData.Stats.VoxelCount++;

}

void Renderer3D::SetGlobalLight(const DirLight& light)
{
    SetDirLightUniforms(light);
}

void Renderer3D::SetPointLightUniforms(const PointLight& light, uint32_t index)
{
    sData.BatchShader->Bind();
    sData.BatchShader->SetUniformFloat3("u_PointLights[" + std::to_string(index) + "].Position", light.Position);
    sData.BatchShader->SetUniformFloat( "u_PointLights[" + std::to_string(index) + "].Constant", light.Constant);
    sData.BatchShader->SetUniformFloat( "u_PointLights[" + std::to_string(index) + "].Linear", light.Linear);
    sData.BatchShader->SetUniformFloat( "u_PointLights[" + std::to_string(index) + "].Quadratic", light.Quadratic);
    sData.BatchShader->SetUniformFloat3("u_PointLights[" + std::to_string(index) + "].Ambient", light.Ambient);
    sData.BatchShader->SetUniformFloat3("u_PointLights[" + std::to_string(index) + "].Diffuse", light.Diffuse);
    sData.BatchShader->SetUniformFloat3("u_PointLights[" + std::to_string(index) + "].Specular", light.Specular);
}

void Renderer3D::SetSpotLightUniforms(const SpotLight& light)
{
    sData.BatchShader->Bind();
    sData.BatchShader->SetUniformFloat3("u_SpotLight.Position", light.Position);
    sData.BatchShader->SetUniformFloat3("u_SpotLight.Direction", light.Direction);
    sData.BatchShader->SetUniformFloat("u_SpotLight.CutOff", light.CutOff);
    sData.BatchShader->SetUniformFloat("u_SpotLight.OuterCutOff", light.OuterCutOff);
    sData.BatchShader->SetUniformFloat("u_SpotLight.Constant", light.Constant);
    sData.BatchShader->SetUniformFloat("u_SpotLight.Linear", light.Linear);
    sData.BatchShader->SetUniformFloat("u_SpotLight.Quadratic", light.Quadratic);
    sData.BatchShader->SetUniformFloat3("u_SpotLight.Ambient", light.Ambient);
    sData.BatchShader->SetUniformFloat3("u_SpotLight.Diffuse", light.Diffuse);
    sData.BatchShader->SetUniformFloat3("u_SpotLight.Specular", light.Specular);
}

void Renderer3D::SetDirLightUniforms(const DirLight& light)
{
    sData.BatchShader->Bind();
    sData.BatchShader->SetUniformFloat3("u_DirLight.Direction", light.Direction);
    sData.BatchShader->SetUniformFloat3("u_DirLight.Ambient", light.Ambient);
    sData.BatchShader->SetUniformFloat3("u_DirLight.Diffuse", light.Diffuse);
    sData.BatchShader->SetUniformFloat3("u_DirLight.Specular", light.Specular);
}

void Renderer3D::SetGamma(float* gamma)
{
    sData.Gamma = gamma;
}

const RendererStats& Renderer3D::GetStats()
{
    return sData.Stats;
}

} // namespace acid