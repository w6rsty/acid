#include "scene/scene.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "renderer/light/light.hpp"

namespace acid
{

Scene::Scene()
{
    Init();
}

void Scene::Init()
{
    camera_ = CreateRef<SceneCamera>();
    camera_->SetOrthographic(1280.0f / 720.0f, 2.0f, -20.0f, 20.0f);
    camera_->SetPerspective(1280.0f / 720.0f, 45.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 0.0f, 10.0f});

    gamma_ = 1.0f;

    for (int z = 0; z < voxelCount_; z++)
    {
        for (int x = 0; x < voxelCount_; x++)
        {
            int i = z * voxelCount_ + x;
            bool isEven = (x + z) % 2 == 0;
            transforms_[i] = glm::translate(glm::mat4(1.0f),
                {
                    (x - voxelCount_ / 2.0f) * 0.25f,
                    -1.0f,
                    (z - voxelCount_ / 2.0f) * 0.25f}
                );
            colors_[i] = isEven ? glm::vec4(1.0f) : glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        }



        for (int i = 0; i < 4; i++)
        {
            pointLights_[i].Position = { 0.0f, 0.0f, 0.0f };
            pointLights_[i].Constant = 1.0f;
            pointLights_[i].Linear = 0.09f;
            pointLights_[i].Quadratic = 0.032f;
            pointLights_[i].Ambient = { 0.05f, 0.05f, 0.05f };
            pointLights_[i].Diffuse = { 1.0f, 0.9f, 0.7f };
            pointLights_[i].Specular = { 0.2f, 0.2f, 0.2f };
        }

        dirLight_.Direction = { 10.0f, -1.0f, 10.0f };
        dirLight_.Ambient = { 0.02f, 0.02f, 0.02f };
        dirLight_.Diffuse = { 0.4f, 0.5f, 0.6f };
        dirLight_.Specular = { 0.2f, 0.2f, 0.2f };

        spotLight_.Direction = { 0.0f, -1.0f, 0.0f };
        spotLight_.CutOff = glm::cos(glm::radians(12.5f));
        spotLight_.OuterCutOff = glm::cos(glm::radians(15.0f));
        spotLight_.Constant = 1.0f;
        spotLight_.Linear = 0.09f;
        spotLight_.Quadratic = 0.032f;
        spotLight_.Ambient = { 0.02f, 0.02f, 0.02f };
        spotLight_.Diffuse = { 1.0f, 0.9f, 0.7f };
        spotLight_.Specular = { 0.2f, 0.2f, 0.2f };
    }
}

void Scene::OnUpdate()
{
    Renderer3D::SetGamma(&gamma_);
    Renderer3D::DrawPointLight({ -4.0f, 0.0f, -4.0f }, pointLights_[0], 0);
    Renderer3D::DrawPointLight({  4.0f, 0.0f,  4.0f }, pointLights_[1], 1);
    Renderer3D::DrawPointLight({ -4.0f, 0.0f,  4.0f }, pointLights_[2], 2);
    Renderer3D::DrawPointLight({  4.0f, 0.0f, -4.0f }, pointLights_[3], 3);

    Renderer3D::DrawSpotLight({ 0.0f, 0.0f, 0.0f }, spotLight_);

    Renderer3D::SetGlobalLight(dirLight_);

    for (int i = 0; i < voxelCount_ * voxelCount_; i++)
    {
        Renderer3D::DrawVoxel(transforms_[i], colors_[i]);
    }

    for (int i = 0; i < voxelCount_ * voxelCount_; i++)
    {
        glm::mat4 up = transforms_[i];
        up[3].y = 1.0f;
        Renderer3D::DrawVoxel(up, colors_[i]);
    }
}

} // namespace acid