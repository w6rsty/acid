#include "renderer/renderer3d.hpp"

#include "core/base.hpp"
#include "core/assert.hpp"
#include "renderer/shader.hpp"
#include "geometry/cuboid.hpp"
#include "renderer/buffer.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/light/light.hpp"
#include "renderer/camera/camera.hpp"
#include "renderer/renderer_enum.hpp"
#include "renderer/renderer_command.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <array>
#include <vector>
#include <functional>

namespace acid
{

struct RendererData
{
    RendererStats Stats;
    const uint32_t MaxPointLights = 16;
    const uint32_t MaxSpotLights = 16;
    const uint32_t MaxDirLights = 4;

    Ref<VertexArray> VoxelVAO = nullptr;
    Ref<Shader> FlatColorShader = nullptr;
    Ref<Shader> TextureShader = nullptr;

    Ref<UniformBuffer> MatrixUBO = nullptr;
    Ref<UniformBuffer> LightUBO = nullptr;
    uint32_t LightUBOSize = POINT_LIGHT_STD_140_SIZE * MaxPointLights + SPOT_LIGHT_STD_140_SIZE * MaxSpotLights + DIR_LIGHT_STD_140_SIZE * MaxDirLights + sizeof(uint32_t) * 3;
    uint32_t PointLightIndex = 0;
    uint32_t PointLightIndexPos = LightUBOSize - sizeof(uint32_t) * 3;
    uint32_t PointLightBufferOffset = 0;
    uint32_t SpotLightIndex = 0;
    uint32_t SpotLightIndexPos = LightUBOSize - sizeof(uint32_t) * 2;
    uint32_t SpotLightBufferOffset = POINT_LIGHT_STD_140_SIZE * MaxPointLights;
    uint32_t DirLightIndex = 0;
    uint32_t DirLightIndexPos = LightUBOSize - sizeof(uint32_t) * 1;
    uint32_t DirLightBufferOffset = POINT_LIGHT_STD_140_SIZE * MaxPointLights + SPOT_LIGHT_STD_140_SIZE * MaxSpotLights;
    std::vector<PointLight> PointLights;
    std::vector<SpotLight> SpotLights;
    std::vector<DirLight> DirLights;

    std::function<void()> SetUniformData = [&]() {
        MatrixUBO->SetData(&Camera->GetProjectionMatrix(), sizeof(glm::mat4));
        MatrixUBO->SetData(&Camera->GetViewMatrix(), sizeof(glm::mat4), sizeof(glm::mat4));

        for (auto& light : PointLights)
        {
            LightUBO->SetData(&light, POINT_LIGHT_STD_140_SIZE, PointLightBufferOffset);
            PointLightBufferOffset += POINT_LIGHT_STD_140_SIZE;
            PointLightIndex++;
            LightUBO->SetData(&PointLightIndex, sizeof(uint32_t), PointLightIndexPos);
        }
        for (auto& light : SpotLights)
        {
            LightUBO->SetData(&light, SPOT_LIGHT_STD_140_SIZE, SpotLightBufferOffset);
            SpotLightBufferOffset += SPOT_LIGHT_STD_140_SIZE;
            SpotLightIndex++;
            LightUBO->SetData(&SpotLightIndex, sizeof(uint32_t), SpotLightIndexPos);
        }
        for (auto& light : DirLights)
        {
            LightUBO->SetData(&light, DIR_LIGHT_STD_140_SIZE, DirLightBufferOffset);
            DirLightBufferOffset += DIR_LIGHT_STD_140_SIZE;
            DirLightIndex++;
            LightUBO->SetData(&DirLightIndex, sizeof(uint32_t), DirLightIndexPos);
        }

        FlatColorShader->BindUniformBlock("Matrices", 0);
        TextureShader->BindUniformBlock("Matrices", 0);

        FlatColorShader->BindUniformBlock("Lights", 1);
        TextureShader->BindUniformBlock("Lights", 1);

        // AC_LOG_TRACE("Set uniform buffer"); // FIXME : multiple call
    };
    
    Ref<SceneCamera> Camera = nullptr;

    Ref<VertexArray> BatchVAO = nullptr;
};

static RendererData sData;

void Renderer3D::Init()
{
    sData.Stats = {};

    {
        Ref<VertexArray> vao = VertexArray::Create();
        vao->Bind();
        
        Ref<VertexBuffer> vbo = VertexBuffer::Create(Cuboid::Packed, sizeof(Cuboid::Packed));
        vbo->SetLayout(CreateRef<VertexBufferLayoutInterleave>(std::initializer_list<VertexBufferElement>{
            { VertexAttributeDataType::Float3, "a_Position" },
            { VertexAttributeDataType::Float3, "a_Normal" },
            { VertexAttributeDataType::Float2, "a_TexCoord" },
        }));
        vao->SetVertexBuffer(vbo);

        uint32_t indices[36];
        uint32_t offset = 0;
        for (int i = 0; i < 36; i += 6)
        {
            indices[i + 0] = offset + 0; 
            indices[i + 1] = offset + 1; 
            indices[i + 2] = offset + 2; 
            indices[i + 3] = offset + 2; 
            indices[i + 4] = offset + 3; 
            indices[i + 5] = offset + 0; 
            offset += 4;
        }
        Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, sizeof(indices));
        vao->SetIndexBuffer(ibo);
        
        sData.VoxelVAO = vao;
    }
    {
        sData.FlatColorShader = Shader::Create("assets/shaders/flat_color.shader");
        sData.TextureShader = Shader::Create("assets/shaders/texture.shader");
    }
    {
        sData.MatrixUBO = UniformBuffer::Create(2 * sizeof(glm::mat4), 0);

        sData.LightUBO = UniformBuffer::Create(sData.LightUBOSize, 1);
    }
    {
        Ref<VertexArray> vao = VertexArray::Create();
        vao->Bind();
        
        uint32_t vsize = 24 * (sizeof(Cuboid::Vertices) + sizeof(Cuboid::Normals) + sizeof(Cuboid::TexCoords));
        uint32_t voffset = 0;
        
        Ref<VertexBuffer> vbo = VertexBuffer::Create(Cuboid::Packed, sizeof(Cuboid::Packed));

        vbo->SetData(Cuboid::Vertices, sizeof(Cuboid::Vertices), voffset);
        voffset += sizeof(Cuboid::Vertices);
        vbo->SetData(Cuboid::Normals, sizeof(Cuboid::Normals), voffset);
        voffset += sizeof(Cuboid::Normals);
        vbo->SetData(Cuboid::TexCoords, sizeof(Cuboid::TexCoords), voffset);
        vbo->SetLayout(CreateRef<VertexBufferLayoutBatch>(std::initializer_list<VertexBufferElement>{
            { VertexAttributeDataType::Float3, "a_Position", 24 },
            { VertexAttributeDataType::Float3, "a_Normal", 24 },
            { VertexAttributeDataType::Float2, "a_TexCoord", 24 },
        }));
        vao->SetVertexBuffer(vbo);

        uint32_t indices[36];
        uint32_t offset = 0;
        for (int i = 0; i < 36; i += 6)
        {
            indices[i + 0] = offset + 0; 
            indices[i + 1] = offset + 1; 
            indices[i + 2] = offset + 2; 
            indices[i + 3] = offset + 2; 
            indices[i + 4] = offset + 3; 
            indices[i + 5] = offset + 0; 
            offset += 4;
        }
        Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, sizeof(indices));
        vao->SetIndexBuffer(ibo);
        
        sData.BatchVAO = vao;
    }
}

void Renderer3D::Shutdown()
{

}

void Renderer3D::BeginScene(const Ref<SceneCamera>& camera)
{
    sData.Stats.Reset();

    if (!sData.Camera) {
        sData.Camera = camera;
        sData.SetUniformData();
    }
}

void Renderer3D::EndScene()
{   
    sData.Camera = nullptr;
    sData.PointLights.clear();
    sData.SpotLights.clear();
    sData.DirLights.clear();
}

void Renderer3D::DrawCuboid(const glm::mat4& transform, const glm::vec4& color)
{
    sData.FlatColorShader->Bind();

    sData.FlatColorShader->SetUniformMat4("u_Model", transform);
    sData.FlatColorShader->SetUniformFloat4("u_Tint", color);
    sData.FlatColorShader->SetUniformFloat3("u_CamPos", sData.Camera->GetPosition());

    RendererCommand::DrawIndexed(sData.VoxelVAO, 36);
    
    sData.Stats.DrawCalls++;
    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
}

void Renderer3D::DrawCuboid(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tint)
{
    sData.TextureShader->Bind();

    sData.TextureShader->SetUniformMat4("u_Model", transform);
    sData.TextureShader->SetUniformFloat4("u_Tint", tint);
    sData.FlatColorShader->SetUniformFloat3("u_CamPos", sData.Camera->GetPosition());

    texture->Bind();
    RendererCommand::DrawIndexed(sData.VoxelVAO, 36);
    
    sData.Stats.DrawCalls++;
    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
}

void Renderer3D::DrawCuboidBatch(const glm::mat4& transform, const glm::vec4& color)
{
    sData.FlatColorShader->Bind();

    sData.FlatColorShader->SetUniformMat4("u_Model", transform);
    sData.FlatColorShader->SetUniformFloat4("u_Tint", color);
    sData.FlatColorShader->SetUniformFloat3("u_CamPos", sData.Camera->GetPosition());

    RendererCommand::DrawIndexed(sData.BatchVAO, 36);
    
    sData.Stats.DrawCalls++;
    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
}

void Renderer3D::DrawModel(const Ref<GltfModel>& model, const glm::mat4& transform, const glm::vec4& color)
{
    sData.TextureShader->Bind();

    sData.TextureShader->SetUniformMat4("u_Model", transform);
    sData.TextureShader->SetUniformFloat4("u_Tint", color);
    sData.FlatColorShader->SetUniformFloat3("u_CamPos", sData.Camera->GetPosition());

    RendererCommand::DrawIndexed(model->GetVertexArray(), model->GetVertexArray()->GetIndexCount());
    
    sData.Stats.DrawCalls++;
    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
}


void Renderer3D::SetPointLight(const PointLight& light)
{
    if (sData.PointLightIndex + 1 >= sData.MaxPointLights) 
    {
        AC_LOG_WARN("Max point lights reached(", sData.MaxPointLights, ")");
        return;
    }
    sData.PointLights.push_back(light);
}

void Renderer3D::SetSpotLight(const SpotLight& light)
{
    if (sData.SpotLightIndex + 1 >= sData.MaxSpotLights) 
    {
        AC_LOG_WARN("Max spot lights reached(", sData.MaxSpotLights, ")");
        return;
    }
    sData.SpotLights.push_back(light);
}

void Renderer3D::SetDirLight(const DirLight& light)
{
    if (sData.DirLightIndex + 1 >= sData.MaxDirLights) 
    {
        AC_LOG_WARN("Max dir lights reached(", sData.MaxDirLights, ")");
        return;
    }
    sData.DirLights.push_back(light);
}

const RendererStats& Renderer3D::GetStats()
{
    return sData.Stats;
}

} // namespace acid