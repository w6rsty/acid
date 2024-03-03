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
    scene_ = CreateRef<Scene>();

    cameraPos_ = scene_->GetCamera()->GetPosition();
    cameraRot_ = scene_->GetCamera()->GetRotation();

    FrameBufferSpecification spec;
    spec.Width = 800;
    spec.Height = 800;

    frameBuffer_ = FrameBuffer::Create(spec);
}

void RenderEngine::Run()
{
    frameBuffer_->Bind();
    RendererCommand::Clear();
    Renderer3D::BeginScene(scene_->GetCamera());
    scene_->OnUpdate();
    Renderer3D::EndScene();
    frameBuffer_->Unbind();
    RendererCommand::Clear();

    // dockspace

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Settings", nullptr);

    auto& stats = Renderer3D::GetStats();
    ImGui::Text("Renderer3D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Triangles: %d", stats.GetTotalTriangleCount());
    ImGui::Text("Voxels: %d", stats.VoxelCount);

    ImGui::Separator();
    if (ImGui::DragFloat3("Camera Position", glm::value_ptr(cameraPos_), 0.1f))
    {
        scene_->GetCamera()->SetPosition(cameraPos_);
    }

    if (ImGui::DragFloat3("Camera Rotation", glm::value_ptr(cameraRot_), 0.1f))
    {
        scene_->GetCamera()->SetRotation(cameraRot_);
    }

    ProjectionType type[2] = {ProjectionType::Perspective, ProjectionType::Orthographic};

    if (ImGui::Combo("Projection Type", &projectionTypeIndex_, "Perspective\0Orthographic\0"))
        scene_->GetCamera()->SetProjectionType(type[projectionTypeIndex_]);

    ImGui::Separator();

    ImGui::Text("Global Light:");
    (ImGui::DragFloat3("Direction", glm::value_ptr(scene_->dirLight_.Direction), 0.1f));
    ImGui::ColorEdit3("Ambient", glm::value_ptr(scene_->dirLight_.Ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(scene_->dirLight_.Diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(scene_->dirLight_.Specular));

    ImGui::Separator();
    ImGui::SliderFloat("Gamma", &scene_->gamma_, 0.1f, 5.0f);
    ImGui::End();

    ImGui::Begin("Scene");
    ImGui::Image(reinterpret_cast<void*>(frameBuffer_->GetColorAttachmentID()), ImVec2(650, 650), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

} // namespace acid