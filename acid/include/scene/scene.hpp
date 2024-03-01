#pragma once

#include "renderer/texture.hpp"
#include "renderer/camera/scene_camera.hpp"
#include "renderer/renderer.hpp"
#include "renderer/light/light.hpp"

#include "glm/glm.hpp"

namespace acid
{

class Scene
{
public:
    Scene();

    void Init();
    void OnUpdate();

    Ref<SceneCamera> GetCamera() { return camera_; }
private:
    const static int voxelCount_ = 20;
    glm::mat4 transforms_[voxelCount_ * voxelCount_];
    glm::vec4 colors_[voxelCount_ * voxelCount_];
    
    Light lights_[4];

    Ref<SceneCamera> camera_;
};

} // namespace acid