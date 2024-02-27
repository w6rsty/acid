#include "renderer/renderer.hpp"

#include "core/log.hpp"
#include "renderer/renderer_command.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "geometry/geo.hpp"

namespace acid
{

struct RendererData
{
    static const uint32_t MaxVertices = 8 * 2000;
    static const uint32_t MaxIndices = 36 * 2000;

    Ref<VertexArray> BatchVA;
    Ref<Shader> Shader;

    Vertex* BatchVertexBufferBase = nullptr;
    Vertex* BatchVertexBufferPtr = nullptr;

    uint32_t* BatchIndexBufferBase = nullptr;
    uint32_t* BatchIndexBufferPtr = nullptr;

    uint32_t VertexCount = 0;
};

static RendererData s_Data;

void Renderer::Init()
{
    RendererCommand::Init(); // Should it be here?

    s_Data.BatchVA = VertexArray::Create();
    s_Data.BatchVA->Bind();
    
    auto vertexBuffer = VertexBuffer::Create(RendererData::MaxVertices * sizeof(Vertex));
    vertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" },
        { VertexDataType::Float3, "a_Normal" },
        { VertexDataType::Float2, "a_TexCoord" }
    });
    s_Data.BatchVA->SetVertexBuffer(vertexBuffer);
    s_Data.BatchVertexBufferBase = new Vertex[s_Data.MaxVertices];
  
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(RendererData::MaxIndices * sizeof(uint32_t));
    s_Data.BatchVA->SetIndexBuffer(indexBuffer);
    s_Data.BatchIndexBufferBase = new uint32_t[s_Data.MaxIndices];

    s_Data.Shader = Shader::Create("assets/shaders/plain.shader");
}

void Renderer::Shutdown()
{
    RendererCommand::Shutdown();
}

void Renderer::BeginScene(const Ref<SceneCamera>& camera)
{
    s_Data.Shader->Bind();
    s_Data.Shader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());
    s_Data.Shader->SetUniformMat4("u_View", camera->GetViewMatrix());

    s_Data.BatchVertexBufferPtr = s_Data.BatchVertexBufferBase;
    s_Data.BatchIndexBufferPtr = s_Data.BatchIndexBufferBase;

    s_Data.VertexCount = 0;
}

void Renderer::EndScene()
{
    uint32_t vertexCount = (uint32_t)((uint8_t*)s_Data.BatchVertexBufferPtr - (uint8_t*)s_Data.BatchVertexBufferBase);
    s_Data.BatchVA->GetVertexBuffer()->SetData(s_Data.BatchVertexBufferBase, vertexCount * sizeof(Vertex));

    uint32_t indexCount = (uint32_t)((uint8_t*)s_Data.BatchIndexBufferPtr - (uint8_t*)s_Data.BatchIndexBufferBase);
    s_Data.BatchVA->GetIndexBuffer()->SetData(s_Data.BatchIndexBufferBase, indexCount * sizeof(uint32_t));

    s_Data.BatchVA->Bind();
    RendererCommand::DrawIndexed(s_Data.BatchVA, indexCount);
}

void Renderer::DrawCuboid(const glm::mat4& transform)
{
    geo::GeoData drawInfo = geo::Cuboid::UnitData();

    for (uint32_t i = 0; i < drawInfo.Indices.size(); i++)
    {
        *s_Data.BatchIndexBufferPtr = drawInfo.Indices[i] + s_Data.VertexCount;
        s_Data.BatchIndexBufferPtr++;
    }

    for (uint32_t i = 0; i < drawInfo.Vertices.size(); i++)
    {
        s_Data.BatchVertexBufferPtr->Position = transform * glm::vec4(drawInfo.Vertices[i].Position, 1.0f);
        s_Data.BatchVertexBufferPtr->Normal = transform * glm::vec4(drawInfo.Vertices[i].Normal, 0.0f);
        s_Data.BatchVertexBufferPtr->TexCoord = drawInfo.Vertices[i].TexCoord;
        s_Data.BatchVertexBufferPtr++;
    }
    s_Data.VertexCount += 8;

}

} // namespace acid