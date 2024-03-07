#include "app/render_engine.hpp"

#include "renderer/buffer.hpp"
#include "renderer/camera/camera.hpp"
#include "renderer/renderer.hpp"
#include "renderer/renderer3d.hpp"
#include "renderer/renderer_command.hpp"

#include "imgui.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace acid
{

RenderEngine::RenderEngine()
{
    Init(); // Init Engine self
    Renderer::Init(); // Init Renderer
} 

RenderEngine::~RenderEngine()
{
    running_ = false;
}

void RenderEngine::Init()
{
    currentScene_ = CreateRef<Scene>();

    FrameBufferSpecification spec;
    spec.Width = 800;
    spec.Height = 600;
    frameBuffer_ = FrameBuffer::Create(spec);

    cameraData_.Pos = currentScene_->GetCamera()->GetPosition();
    cameraData_.Rot = currentScene_->GetCamera()->GetRotation();
}

void RenderEngine::LoadScene(const Ref<Scene>& scene)
{
    
}

void RenderEngine::Run()
{
    frameBuffer_->Bind();
    RendererCommand::Clear();

    Renderer3D::BeginScene(currentScene_->GetCamera());

    currentScene_->OnUpdate();
    
    Renderer3D::EndScene();
    frameBuffer_->Unbind();

    // dockspace
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Settings", nullptr);

    auto& stats = Renderer3D::GetStats();
    ImGui::Text("Renderer3D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Vertices: %d", stats.VertexCount);
    ImGui::Text("Triangles: %d", stats.TriangleCount);

    ImGui::Separator();
    if (ImGui::DragFloat3("Cam Pos", glm::value_ptr(cameraData_.Pos), 0.1f))
    {
        currentScene_->GetCamera()->SetPosition(cameraData_.Pos);
    }

    if (ImGui::DragFloat3("Cam Rot", glm::value_ptr(cameraData_.Rot), 0.1f))
    {
        currentScene_->GetCamera()->SetRotation(cameraData_.Rot);
    }

    ProjectionType type[2] = {ProjectionType::Perspective, ProjectionType::Orthographic};

    if (ImGui::Combo("Projection", &cameraData_.ProjectionTypeIndex, "Perspective\0Orthographic\0"))
        currentScene_->GetCamera()->SetProjectionType(type[cameraData_.ProjectionTypeIndex]);

    ImGui::End();

    ImGui::Begin("Scene");
    ImGui::Image(reinterpret_cast<void*>(frameBuffer_->GetColorAttachmentID()), ImVec2(800, 600), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

} // namespace acid