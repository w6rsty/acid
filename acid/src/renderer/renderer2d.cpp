#include "renderer/renderer2d.hpp"

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

struct CircleVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    float Radius;
};

struct Renderer2DData
{
    const uint32_t MaxQuads = 1000;
    const uint32_t MaxQuadVertices = MaxQuads * 4;
    const uint32_t MaxQuadIndices = MaxQuads * 6;

    const uint32_t MaxCircles = 1000;
    const uint32_t MaxCircleVertices = MaxCircles;
    const uint32_t MaxCircleIndices = MaxCircles;

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> QuadShader;

    QuadVertex* QuadVertexBase = nullptr;
    QuadVertex* QuadVertexPtr = nullptr;

    uint32_t QuadIndexCount = 0;

    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer;
    Ref<Shader> CircleShader;

    CircleVertex* CircleVertexBase = nullptr;
    CircleVertex* CircleVertexPtr = nullptr;

    uint32_t CircleIndexCount = 0;

    glm::vec3 QuadVertexPositions[4] = {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        { -0.5f,  0.5f, 0.0f },
    };

    glm::vec2 QuadTexCoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };
};

static Renderer2DData sData;

void Renderer2D::Init()
{
    
    auto quadVertexArray = VertexArray::Create();
    quadVertexArray->Bind();

    auto quadVertexBuffer = VertexBuffer::Create(sData.MaxQuadVertices * sizeof(QuadVertex));
    quadVertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" },
        { VertexDataType::Float4, "a_Color" },
        { VertexDataType::Float2, "a_TexCoord" },
    });
    quadVertexArray->SetVertexBuffer(quadVertexBuffer);
    sData.QuadVertexBase = new QuadVertex[sData.MaxQuadVertices];
    
    uint32_t* quadIndices = new uint32_t[sData.MaxQuadIndices];
    uint32_t quadOffset = 0;
    for (uint32_t i = 0; i < sData.MaxQuadIndices; i += 6)
    {
        quadIndices[i + 0] = 0 + quadOffset;
        quadIndices[i + 1] = 1 + quadOffset;
        quadIndices[i + 2] = 2 + quadOffset;
        quadIndices[i + 3] = 2 + quadOffset;
        quadIndices[i + 4] = 3 + quadOffset;
        quadIndices[i + 5] = 0 + quadOffset;

        quadOffset += 4;
    }

    auto quadIndexBuffer = IndexBuffer::Create(quadIndices, sizeof(quadIndices));
    quadVertexArray->SetIndexBuffer(quadIndexBuffer);
    delete[] quadIndices;

    sData.QuadVertexArray = quadVertexArray;
    sData.QuadVertexBuffer = quadVertexBuffer;

    auto circleVertexArray = VertexArray::Create();
    circleVertexArray->Bind();

    auto circleVertexBuffer = VertexBuffer::Create(sData.MaxCircleVertices * sizeof(CircleVertex));
    circleVertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" },
        { VertexDataType::Float4, "a_Color" },
        { VertexDataType::Float, "a_Radius" },
    });
    circleVertexArray->SetVertexBuffer(circleVertexBuffer);
    sData.CircleVertexBase = new CircleVertex[sData.MaxCircleVertices];
    
    circleVertexArray->SetIndexBuffer(quadIndexBuffer);

    sData.CircleVertexArray = circleVertexArray;
    sData.CircleVertexBuffer = circleVertexBuffer;

    sData.QuadShader = Shader::Create("assets/shaders/geo2d/quad.shader");
    sData.CircleShader = Shader::Create("assets/shaders/geo2d/circle.shader");
}

void Renderer2D::Shutdown()
{

}

void Renderer2D::BeginScene(const Ref<SceneCamera> &camera)
{
    sData.QuadShader->Bind();
    sData.QuadShader->SetUniformMat4("u_View", camera->GetViewMatrix());
    sData.QuadShader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());

    sData.CircleShader->Bind();
    sData.CircleShader->SetUniformMat4("u_View", camera->GetViewMatrix());
    sData.CircleShader->SetUniformMat4("u_Projection", camera->GetProjectionMatrix());

    sData.QuadVertexPtr = sData.QuadVertexBase;
    sData.CircleVertexPtr = sData.CircleVertexBase;

    sData.QuadIndexCount = 0;
    sData.CircleIndexCount = 0;
}

void Renderer2D::EndScene()
{
    if (sData.QuadIndexCount > 0)
    {
        sData.QuadVertexArray->Bind();
        sData.QuadVertexBuffer->SetData(sData.QuadVertexBase, sData.QuadIndexCount * sizeof(QuadVertex));
        RendererCommand::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
    }

    if (sData.CircleIndexCount > 0)
    {
        sData.CircleVertexArray->Bind();
        sData.CircleVertexBuffer->SetData(sData.CircleVertexBase, sData.CircleIndexCount * sizeof(CircleVertex));
        RendererCommand::DrawIndexed(sData.CircleVertexArray, sData.CircleIndexCount);
    }
}

void Renderer2D::DrawQuad(const glm::mat4 &transform)
{

    for (uint32_t i = 0; i < 4; i++)
    {
        sData.QuadVertexPtr->Position = transform * glm::vec4(sData.QuadVertexPositions[i], 1.0f);
        sData.QuadVertexPtr->Color = { 0.8f, 0.2f, 0.3f, 1.0f };
        sData.QuadVertexPtr->TexCoord = sData.QuadTexCoords[i];
        sData.QuadVertexPtr++;
    }

    sData.QuadIndexCount += 6;
}

void Renderer2D::DrawCircle(const glm::mat4 &transform)
{
    sData.CircleVertexPtr->Position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    sData.CircleVertexPtr->Color = { 0.2f, 0.3f, 0.8f, 1.0f };
    sData.CircleVertexPtr->Radius = 0.5f;
    sData.CircleVertexPtr++;

    sData.CircleIndexCount += 1;
}

} // namespace