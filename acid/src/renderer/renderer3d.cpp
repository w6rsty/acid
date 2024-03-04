#include "renderer/renderer3d.hpp"

#include "core/assert.hpp"
#include "core/log.hpp"
#include "geometry/cuboid.hpp"
#include "renderer/buffer.hpp"
#include "renderer/camera/camera.hpp"
#include "renderer/light/light.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/renderer_command.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <array>
#include <vector>
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
    RendererStats Stats;
    const uint32_t MaxPointLights = 10;

    Ref<VertexArray> VoxelVAO = nullptr;
    Ref<Shader> VoxelFlatColorShader = nullptr;
    Ref<Shader> VoxelTextureShader = nullptr;

    Ref<UniformBuffer> MatrixUBO = nullptr;
    Ref<UniformBuffer> LightUBO = nullptr;
    uint32_t LightIndex = 0;
    uint32_t LightBufferOffset = 0;
    std::vector<PointLight> PointLights;

    std::function<void()> SetUniformData = [&]() {
        MatrixUBO->SetData(&Camera->GetProjectionMatrix(), sizeof(glm::mat4));
        MatrixUBO->SetData(&Camera->GetViewMatrix(), sizeof(glm::mat4), sizeof(glm::mat4));

        VoxelFlatColorShader->BindUniformBlock("Matrices", 0);
        VoxelTextureShader->BindUniformBlock("Matrices", 0);

        VoxelFlatColorShader->BindUniformBlock("Lights", 1);
        VoxelTextureShader->BindUniformBlock("Lights", 1);

        for (auto& light : PointLights)
        {
            LightUBO->SetData(&light, POINT_LIGHT_STD_140_SIZE, LightBufferOffset);
            LightBufferOffset += POINT_LIGHT_STD_140_SIZE;
            LightIndex++;
            LightUBO->SetData(&LightIndex, sizeof(uint32_t), POINT_LIGHT_STD_140_SIZE * MaxPointLights);
        }
    };
    
    Ref<SceneCamera> Camera = nullptr;
};

static RendererData sData;

void Renderer3D::Init()
{
    sData.Stats = {};

    {
        Ref<VertexArray> vao = VertexArray::Create();
        vao->Bind();
        
        Ref<VertexBuffer> vbo = VertexBuffer::Create(Cuboid::Packed, sizeof(Cuboid::Packed));
        vbo->SetLayout({
            {VertexDataType::Float3, "a_Position"},
            {VertexDataType::Float3, "a_Normal"},
            {VertexDataType::Float2, "a_TexCoord"},
        });
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
        sData.VoxelFlatColorShader = Shader::Create("assets/shaders/flat_color.shader");
        sData.VoxelTextureShader = Shader::Create("assets/shaders/texture.shader");
    }
    {
        sData.MatrixUBO = UniformBuffer::Create(2 * sizeof(glm::mat4), 0);
        sData.LightUBO = UniformBuffer::Create(POINT_LIGHT_STD_140_SIZE * sData.MaxPointLights + sizeof(uint32_t), 1);
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
}

void Renderer3D::DrawCuboid(const glm::mat4& transform, const glm::vec4& color)
{
    sData.VoxelFlatColorShader->Bind();

    sData.VoxelFlatColorShader->SetUniformMat4("u_Model", transform);
    sData.VoxelFlatColorShader->SetUniformFloat4("u_Tint", color);
    sData.VoxelFlatColorShader->SetUniformFloat3("u_CamPos", sData.Camera->GetPosition());

    RendererCommand::DrawIndexed(sData.VoxelVAO, 36);
    
    sData.Stats.DrawCalls++;
    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
}

void Renderer3D::DrawCuboid(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tint)
{
    sData.VoxelTextureShader->Bind();

    sData.VoxelTextureShader->SetUniformMat4("u_Model", transform);
    sData.VoxelTextureShader->SetUniformFloat4("u_Tint", tint);
    sData.VoxelFlatColorShader->SetUniformFloat3("u_CamPos", sData.Camera->GetPosition());

    texture->Bind();
    RendererCommand::DrawIndexed(sData.VoxelVAO, 36);
    
    sData.Stats.DrawCalls++;
    sData.Stats.VertexCount += 24;
    sData.Stats.TriangleCount += 12;
}

void Renderer3D::SetLight(const PointLight& light)
{
    if (sData.LightIndex + 1 >= sData.MaxPointLights) 
    {
        AC_LOG_WARN("Max point lights reached(", sData.MaxPointLights, ")");
        return;
    }
    sData.PointLights.push_back(light);
}

const RendererStats& Renderer3D::GetStats()
{
    return sData.Stats;
}

} // namespace acid