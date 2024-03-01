#include "engine/engine.hpp"

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

Engine::Engine()
{
    Init(); // Init Engine self
    Renderer::Init(); // Init Renderer
} 

Engine::~Engine()
{
    running_ = false;
}

void Engine::Init()
{
    scene_ = CreateRef<Scene>();

    cameraPos_ = scene_->GetCamera()->GetPosition();
    cameraRot_ = scene_->GetCamera()->GetRotation();
}

void Engine::Run()
{
    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();

    Renderer3D::BeginScene(scene_->GetCamera());

    scene_->OnUpdate();

    Renderer3D::EndScene();

    ImGui::Begin("Settings");

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

    ImGui::End();
}

} // namespace acid