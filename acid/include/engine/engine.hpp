/// TODO: WE NEED AN APPLICATION to run the engine !!!!!
/// TODO: And add imgui !!!
#pragma once

#include "window/window.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera/scene_camera.hpp"

namespace acid
{

/// Driven the core functions
class Engine
{
public:
    Engine();
    ~Engine();

    void Init();

    void OnAttach();
    void OnDetach();

    void Run();
private:
    bool running_ = true;

    Ref<Window> window_;
    Ref<SceneCamera> camera_;
};

} // namespace acid