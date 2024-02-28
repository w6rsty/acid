#include "renderer/renderer3d.hpp"

#include "core/log.hpp"
#include "geometry/geo.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/renderer_command.hpp"

namespace acid
{

struct RendererData
{
    static const uint32_t MaxVertices = 16192;
    static const uint32_t MaxIndices = MaxVertices * 64;

    Ref<VertexArray> BatchVA;
    Ref<Shader> BatchShader;

    Vertex* BatchVertexBufferBase = nullptr;
    Vertex* BatchVertexBufferPtr = nullptr;

    uint32_t* BatchIndexBufferBase = nullptr;
    uint32_t* BatchIndexBufferPtr = nullptr;

    uint32_t VertexCount = 0;

    RendererStats Stats;
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
        { VertexDataType::Float3, "a_Normal" },
        { VertexDataType::Float4, "a_Color" },
        { VertexDataType::Float2, "a_TexCoord" }
    });
    sData.BatchVA->SetVertexBuffer(vertexBuffer);
    sData.BatchVertexBufferBase = new Vertex[sData.MaxVertices];
  
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(RendererData::MaxIndices * sizeof(uint32_t));
    sData.BatchVA->SetIndexBuffer(indexBuffer);
    sData.BatchIndexBufferBase = new uint32_t[sData.MaxIndices];

    sData.BatchShader = Shader::Create("assets/shaders/plain.shader");
}

void Renderer3D::Shutdown()
{
    delete[] sData.BatchVertexBufferBase;
    delete[] sData.BatchIndexBufferBase;
}

void Renderer3D::StartBatch()
{
    sData.BatchVertexBufferPtr = sData.BatchVertexBufferBase;
    sData.BatchIndexBufferPtr = sData.BatchIndexBufferBase;
    sData.VertexCount = 0;
}

void Renderer3D::NextBatch()
{
    Flush();
    StartBatch();
}

void Renderer3D::BeginScene(const Ref<SceneCamera>& camera)
{
    sData.Stats.Reset();
    sData.BatchShader->Bind();
    sData.BatchShader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());
    sData.BatchShader->SetUniformMat4("u_View", camera->GetViewMatrix());

    StartBatch();
}

void Renderer3D::Flush()
{
    if (sData.VertexCount == 0) return;

    sData.BatchVA->GetVertexBuffer()->SetData(sData.BatchVertexBufferBase, sData.VertexCount * sizeof(Vertex));

    uint32_t indexCount = (uint32_t)((uint8_t*)sData.BatchIndexBufferPtr - (uint8_t*)sData.BatchIndexBufferBase);
    sData.BatchVA->GetIndexBuffer()->SetData(sData.BatchIndexBufferBase, indexCount * sizeof(uint32_t));

    RendererCommand::DrawIndexed(sData.BatchVA, indexCount);
    sData.Stats.DrawCalls++;
}

void Renderer3D::EndScene()
{
    Flush();
}

void Renderer3D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    DrawSimpleGeo(geo::Quad::Data, color, transform);
}

void Renderer3D::DrawTriangle(const glm::mat4& transform, const glm::vec4& color)
{
    DrawSimpleGeo(geo::Triangle::Data, color, transform);
}

/// Cost a lot of time. Avoid using it.
void Renderer3D::DrawCircle(const glm::mat4& transform, const glm::vec4& color)
{
    DrawSimpleGeo(geo::Circle::Data, color, transform);
}

void Renderer3D::DrawCuboid(const glm::mat4& transform, const glm::vec4& color)
{
    DrawSimpleGeo(geo::Cuboid::Data, color, transform);
}

void Renderer3D::DrawPyramid(const glm::mat4& transform, const glm::vec4& color)
{
    DrawSimpleGeo(geo::Pyramid::Data, color, transform);
}

void Renderer3D::DrawSimpleGeo(const geo::GeoData& drawInfo, const glm::vec4& color, const glm::mat4& transform)
{
    if (sData.VertexCount + drawInfo.Vertices.size() > RendererData::MaxVertices)
    {
        NextBatch();
    }

    for (const uint32_t index : drawInfo.Indices)
    {
        *sData.BatchIndexBufferPtr = index + sData.VertexCount;
        sData.BatchIndexBufferPtr++;
    }

    for (const Vertex& vertex : drawInfo.Vertices)
    {
        sData.BatchVertexBufferPtr->Position = transform * glm::vec4(vertex.Position, 1.0f);
        sData.BatchVertexBufferPtr->Normal = transform * glm::vec4(vertex.Normal, 0.0f);
        sData.BatchVertexBufferPtr->Color = color;
        sData.BatchVertexBufferPtr->TexCoord = vertex.TexCoord;
        sData.BatchVertexBufferPtr++;
    }

    sData.VertexCount += drawInfo.Vertices.size();
    sData.Stats.VertexCount += drawInfo.Vertices.size();
    sData.Stats.TriangleCount += drawInfo.Indices.size() / 3;
}

const RendererStats& Renderer3D::GetStats()
{
    return sData.Stats;
}

} // namespace acid