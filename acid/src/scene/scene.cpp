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

for (int z = 0; z < voxelCount_; z++)
{
    for (int x = 0; x < voxelCount_; x++)
    {
        int i = z * voxelCount_ + x;
        bool isEven = (x + z) % 2 == 0;
        transforms_[i] = glm::translate(glm::mat4(1.0f),
            {
                (x - voxelCount_ / 2.0f) * 0.25f,
                0.0f,
                (z - voxelCount_ / 2.0f) * 0.25f}
            );
        colors_[i] = isEven ? glm::vec4(1.0f) : glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    }

    camera_ = CreateRef<SceneCamera>();
    camera_->SetOrthographic(1280.0f / 720.0f, 2.0f, -20.0f, 20.0f);
    camera_->SetPerspective(1280.0f / 720.0f, 45.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 2.0f, 10.0f});

    for (int i = 0; i < 4; i++)
    {
        lights_[i].Ambient = { 0.1f, 0.1f, 0.1f };
        lights_[i].AmbientIntensity = 0.1f;
        lights_[i].Diffuse = { 1.0f, 0.9f, 0.7f };
        lights_[i].DiffuseIntensity = 0.8f;
        lights_[i].Specular = { 0.5f, 0.5f, 0.5f };
        lights_[i].SpecularIntensity = 0.5f;
        lights_[i].Shininess = 4.0f;
    }
}

}

void Scene::OnUpdate()
{
    Renderer3D::DrawLight({  4.0f, 0.5f,  4.0f }, lights_[0], 0);
    Renderer3D::DrawLight({ -4.0f, 0.5f,  4.0f }, lights_[1], 1);
    Renderer3D::DrawLight({  4.0f, 0.5f, -4.0f }, lights_[2], 2);
    Renderer3D::DrawLight({ -4.0f, 0.5f, -4.0f }, lights_[3], 3);

    Renderer3D::DrawLight({ 0.0f, 3.0f, 0.0f }, lights_[3], 4);

    for (int i = 0; i < voxelCount_ * voxelCount_; i++)
    {
        Renderer3D::DrawVoxel(transforms_[i], colors_[i]);
    }

    for (int i = 0; i < voxelCount_ * voxelCount_; i++)
    {
        glm::mat4 top = transforms_[i];
        top[3][1] += 4.0f;
        Renderer3D::DrawVoxel(top, colors_[i]);
    }
}

} // namespace acid