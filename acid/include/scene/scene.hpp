#pragma once

#include "renderer/texture.hpp"

#include "glm/glm.hpp"

namespace acid
{

class Scene
{
public:
    Scene();

    void Init();
    void OnUpdate();
private:
    const static int voxelCount_ = 50;
    glm::mat4 transforms_[voxelCount_ * voxelCount_];
    glm::vec4 colors_[voxelCount_ * voxelCount_];

    Ref<Texture> texture_;
};

} // namespace acid