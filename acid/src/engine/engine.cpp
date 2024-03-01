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

    camera_ = CreateRef<SceneCamera>();
    camera_->SetOrthographic(1280.0f / 720.0f, 2.0f, -20.0f, 20.0f);
    camera_->SetPerspective(1280.0f / 720.0f, 30.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 5.0f, 10.0f});
    camera_->SetRotation({-15.0f, 0.0f, 0.0f});
    cameraPos_ = camera_->GetPosition();
    cameraRot_ = camera_->GetRotation();
}

void Engine::Run()
{
    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();

    Renderer3D::BeginScene(camera_);

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
        camera_->SetPosition(cameraPos_);
    }

    if (ImGui::DragFloat3("Camera Rotation", glm::value_ptr(cameraRot_), 0.1f))
    {
        camera_->SetRotation(cameraRot_);
    }

    ProjectionType type[2] = {ProjectionType::Perspective, ProjectionType::Orthographic};

    if (ImGui::Combo("Projection Type", &projectionTypeIndex_, "Perspective\0Orthographic\0"))
        camera_->SetProjectionType(type[projectionTypeIndex_]);

    ImGui::End();
}

} // namespace acid