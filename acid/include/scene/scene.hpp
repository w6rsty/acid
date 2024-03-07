#pragma once

#include "renderer/renderer.hpp"
#include "renderer/light/light.hpp"
#include "renderer/camera/scene_camera.hpp"
#include "renderer/light/material.hpp"

#include "glm/glm.hpp"

namespace acid
{

class Scene
{
public:
    Scene();

    void Setup();
    void OnUpdate();

    Ref<SceneCamera> GetCamera() { return camera_; }
private:
    Ref<SceneCamera> camera_;

    Ref<Texture> texture_;

    DirLight dirLight_;
    Material material_;

    friend class RenderEngine;
};

} // namespace acid