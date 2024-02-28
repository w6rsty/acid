#include "renderer/renderer2d.hpp"

#include "glm/fwd.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/shader.hpp"
#include "renderer/buffer.hpp"
#include "renderer/renderer_command.hpp"

#include <cstdint>

namespace acid
{

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
};

struct Renderer2DData
{
    const uint32_t MaxQuads = 10000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices = MaxQuads * 6;

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> QuadShader;

    uint32_t QuadIndexCount = 0;

    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    const glm::vec3 QuadVertexPositions[4] = {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        { -0.5f,  0.5f, 0.0f }
    };

    const glm::vec2 QuadVertexTexCoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    RendererStats Stats;
};

static Renderer2DData sData;

void Renderer2D::Init()
{
    sData.Stats = {};
    sData.QuadVertexArray = VertexArray::Create();
    sData.QuadVertexArray->Bind();

    sData.QuadVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(QuadVertex));
    sData.QuadVertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" },
        { VertexDataType::Float4, "a_Color" },
        { VertexDataType::Float2, "a_TexCoord" }
    });
    sData.QuadVertexArray->SetVertexBuffer(sData.QuadVertexBuffer);

    sData.QuadVertexBufferBase = new QuadVertex[sData.MaxVertices];
    
    uint32_t* quadIndices = new uint32_t[sData.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < sData.MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;
        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, sData.MaxIndices);
    sData.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
    delete[] quadIndices;

    sData.QuadShader = Shader::Create("assets/shaders/geo2d/quad.shader");
}

void Renderer2D::Shutdown()
{
    delete[] sData.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const Ref<SceneCamera> &camera)
{
    sData.QuadShader->Bind();
    sData.QuadShader->SetUniformMat4("u_View", camera->GetViewMatrix());
    sData.QuadShader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());

    sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
    sData.QuadIndexCount = 0;
}

void Renderer2D::Flush()
{
    if (sData.QuadIndexCount == 0) return;

    uint32_t dataSize = (uint8_t*)sData.QuadVertexBufferPtr - (uint8_t*)sData.QuadVertexBufferBase;
    sData.QuadVertexBuffer->SetData(sData.QuadVertexBufferBase, dataSize);

    RendererCommand::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
    sData.Stats.DrawCalls++;
}

void Renderer2D::EndScene()
{
    Flush();
    sData.Stats.Reset();
}

void Renderer2D::DrawQuad(const glm::mat4 &transform)
{
    for (uint32_t i = 0; i < 4; i++)
    {
        sData.QuadVertexBufferPtr->Position = transform * glm::vec4(sData.QuadVertexPositions[i], 1.0f);
        sData.QuadVertexBufferPtr->Color = { 0.8f, 0.2f, 0.3f, 1.0f };
        sData.QuadVertexBufferPtr->TexCoord = sData.QuadVertexTexCoords[i];
        sData.QuadVertexBufferPtr++;
    }

    sData.QuadIndexCount += 6;
    sData.Stats.VertexCount += 4;
    sData.Stats.TriangleCount += 2;
}

const RendererStats &Renderer2D::GetStats()
{
    return sData.Stats;
}

} // namespace