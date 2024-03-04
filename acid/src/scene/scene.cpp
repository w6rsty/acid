#include "scene/scene.hpp"

#include "renderer/renderer3d.hpp"

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
    camera_->SetPerspective(1.0f, 45.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 1.0f, 5.0f});

    texture_ = Texture2D::Create("assets/textures/kobe.png");

    for (int i = 0; i < 4; i++)
    {
        lights_[i] = PointLight {
            .Position = {0.0f, 1.0f, 0.0f},
            .Constant = 1.0f,
            .Linear = 0.9f,
            .Quadratic = 0.032f,
            .Ambient = {0.02f, 0.02f, 0.02f},
            .Diffuse = {1.0f, 0.9f, 0.7f},
            .Specular = {0.2f, 0.2f, 0.2f}
        };
    }
    lights_[0].Position = { 1.0f, 2.0f,  1.0f};
    lights_[1].Position = {-1.0f, 2.0f,  1.0f};
    lights_[2].Position = {-1.0f, 2.0f, -1.0f};
    lights_[3].Position = { 1.0f, 2.0f, -1.0f};
    Renderer3D::SetLight(lights_[0]);
    Renderer3D::SetLight(lights_[1]);
    Renderer3D::SetLight(lights_[2]);
    // Renderer3D::SetLight(lights_[3]);
    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    // RendererCommand::DrawWireFrame(true);
}

void Scene::OnUpdate()
{
    for (int i = -3; i <= 3; i++)
    {
        for (int j = -3; j <= 3; j++)
        {
            glm::vec3 pos = {1.0f * i, 0.0f, 1.0f * j};
            glm::vec4 color = (i + j) % 2 == 0 
                ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
                : glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
            Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), pos), color);
        }
    }

    Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), {0, 1, 0}), texture_);
}

} // namespace acid