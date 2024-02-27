#include "renderer/renderer3d.hpp"

#include "core/log.hpp"
#include "renderer/renderer_command.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "geometry/geo.hpp"

namespace acid
{

struct RendererData
{
    static const uint32_t MaxVertices = 10000;
    static const uint32_t MaxIndices = MaxVertices * 6;

    Ref<VertexArray> BatchVA;
    Ref<Shader> BatchShader;

    Vertex* BatchVertexBufferBase = nullptr;
    Vertex* BatchVertexBufferPtr = nullptr;

    uint32_t* BatchIndexBufferBase = nullptr;
    uint32_t* BatchIndexBufferPtr = nullptr;

    uint32_t VertexCount = 0;
};

static RendererData s_Data;

void Renderer3D::Init()
{
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

    s_Data.BatchShader = Shader::Create("assets/shaders/plain.shader");
}

void Renderer3D::Shutdown()
{
    delete[] s_Data.BatchVertexBufferBase;
    delete[] s_Data.BatchIndexBufferBase;
}

void Renderer3D::BeginScene(const Ref<SceneCamera>& camera)
{
    s_Data.BatchShader->Bind();
    s_Data.BatchShader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());
    s_Data.BatchShader->SetUniformMat4("u_View", camera->GetViewMatrix());

    s_Data.BatchVertexBufferPtr = s_Data.BatchVertexBufferBase;
    s_Data.BatchIndexBufferPtr = s_Data.BatchIndexBufferBase;

    s_Data.VertexCount = 0;
}

void Renderer3D::EndScene()
{
    if (s_Data.VertexCount > 0) 
    {
        s_Data.BatchVA->GetVertexBuffer()->SetData(s_Data.BatchVertexBufferBase, s_Data.VertexCount * sizeof(Vertex));

        uint32_t indexCount = (uint32_t)((uint8_t*)s_Data.BatchIndexBufferPtr - (uint8_t*)s_Data.BatchIndexBufferBase);
        s_Data.BatchVA->GetIndexBuffer()->SetData(s_Data.BatchIndexBufferBase, indexCount * sizeof(uint32_t));

        RendererCommand::DrawIndexed(s_Data.BatchVA, indexCount);
    }
}

void Renderer3D::DrawQuad(const glm::mat4& transform)
{
    DrawSimpleGeo(geo::Quad::Data, transform);
}

void Renderer3D::DrawTriangle(const glm::mat4& transform)
{
    DrawSimpleGeo(geo::Triangle::Data, transform);
}

/// Cost a lot of time. Avoid using it.
void Renderer3D::DrawCircle(const glm::mat4& transform)
{
    DrawSimpleGeo(geo::Circle::Data, transform);
}

void Renderer3D::DrawCuboid(const glm::mat4& transform)
{
    DrawSimpleGeo(geo::Cuboid::Data, transform);
}

void Renderer3D::DrawPyramid(const glm::mat4& transform)
{
    DrawSimpleGeo(geo::Pyramid::Data, transform);
}

void Renderer3D::DrawSimpleGeo(const geo::GeoData& drawInfo, const glm::mat4& transform)
{
    for (const uint32_t index : drawInfo.Indices)
    {
        *s_Data.BatchIndexBufferPtr = index + s_Data.VertexCount;
        s_Data.BatchIndexBufferPtr++;
    }

    for (const Vertex& vertex : drawInfo.Vertices)
    {
        s_Data.BatchVertexBufferPtr->Position = transform * glm::vec4(vertex.Position, 1.0f);
        s_Data.BatchVertexBufferPtr->Normal = transform * glm::vec4(vertex.Normal, 0.0f);
        s_Data.BatchVertexBufferPtr->TexCoord = vertex.TexCoord;
        s_Data.BatchVertexBufferPtr++;
    }

    s_Data.VertexCount += drawInfo.Vertices.size();
}

} // namespace acid