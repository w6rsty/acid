#include "scene/scene.hpp"

#include "renderer/renderer.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "renderer/renderer3d.hpp"

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

    texture_ = Texture2D::Create("assets/textures/bulb.png");
}

}

void Scene::OnUpdate()
{
    Renderer3D::DrawSprite(glm::translate(glm::mat4(1.0f), {0, 2, 0}), texture_);

    for (int i = 0; i < voxelCount_ * voxelCount_; i++)
    {
        Renderer3D::DrawVoxel(transforms_[i], colors_[i]);
    }
}

} // namespace acid