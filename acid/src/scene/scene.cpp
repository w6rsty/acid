#include "scene/scene.hpp"

#include "renderer/light/light.hpp"
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
    camera_->SetPerspective(4.0f / 3.0f, 45.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 2.0f, 10.0f});

    texture_ = Texture2D::Create("assets/textures/bulb.png");

    dirLight_ = DirLight {
        .Direction = {-0.2f, -1.0f, -0.3f},
        .Ambient = {0.05f, 0.05f, 0.05f},
        .Diffuse = {0.7f, 0.7f, 0.6f},
        .Specular = {0.5f, 0.5f, 0.5f}
    };

    Renderer3D::SetDirLight(dirLight_);

    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    // RendererCommand::DrawWireFrame(true);
}

void Scene::OnUpdate()
{
    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            glm::vec3 pos = {1.0f * i, 0.0f, 1.0f * j};
            glm::vec4 color = (i + j) % 2 == 0 
                ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
                : glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
            Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), pos), color);
        }
    }

    Renderer3D::DrawCuboid(glm::translate(glm::mat4(1.0f), { 0, 1, 0}), texture_);
}

} // namespace acid