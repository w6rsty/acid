#include "scene/scene.hpp"

#include "renderer/light/light.hpp"
#include "renderer/renderer3d.hpp"
#include "geometry/model.hpp"

#include "glm/ext/matrix_transform.hpp"

namespace acid
{

Scene::Scene()
{
    // read scene

    // load setups
    Setup();
}

void Scene::Setup()
{
    camera_ = CreateRef<SceneCamera>();
    camera_->SetPerspective(4.0f / 3.0f, 45.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 1.0f, 10.0f});

    model_ = CreateRef<GltfModel>("assets/models/cube.gltf");

    texture_ = Texture2D::Create("assets/textures/player.png");

    dirLight_ = DirLight {
        .Direction = {-0.1f, -1.0f, -0.1f},
        .Ambient = {0.05f, 0.05f, 0.05f},
        .Diffuse = {1.0f, 0.9f, 0.7f},
        .Specular = {0.5f, 0.5f, 0.5f}
    };

    Renderer3D::SetDirLight(dirLight_);

    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    // RendererCommand::DrawWireFrame(true);
}

void Scene::OnUpdate()
{
    glm::mat4 model = glm::scale(glm::mat4(1.0f), { 4, 1, 4 });
    // Renderer3D::DrawCuboid(model);
    // Renderer3D::DrawModel(model_);
    Renderer3D::DrawCuboidBatch(model);

    Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), { 0, 1.01, 0}), texture_);

}

} // namespace acid