#pragma once

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
    const static int voxelCount_ = 30;
    glm::mat4 transforms_[voxelCount_ * voxelCount_];
    glm::vec4 colors_[voxelCount_ * voxelCount_];
    
    PointLight pointLights_[4];
    DirLight dirLight_;
    SpotLight spotLight_;
    float gamma_ = 1.2f;

    Ref<SceneCamera> camera_;

    Ref<Texture> texture_;

    friend class RenderEngine;
};

} // namespace acid