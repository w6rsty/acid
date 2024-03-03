#include "scene/scene.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "renderer/light/light.hpp"
#include "renderer/texture.hpp"

namespace acid
{

Scene::Scene()
{
    Init();
}

void Scene::Init()
{
    camera_ = CreateRef<SceneCamera>();
    camera_->SetPerspective(16.0f / 9.0f, 45.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 1.0f, 5.0f});

    texture_ = Texture2D::Create("assets/textures/bulb.png");

    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    // RendererCommand::DrawWireFrame(true);
}

void Scene::OnUpdate()
{
    Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f}), glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
    Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), {1.0f, 0.0f, 0.0f}), texture_, glm::vec4(1.0f));
}

} // namespace acid