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
    Ref<SceneCamera> camera_;

    Ref<Texture> texture_;

    friend class RenderEngine;
};

} // namespace acid