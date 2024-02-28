/// TODO: WE NEED AN APPLICATION to run the engine !!!!!
/// TODO: And add imgui !!!
#pragma once

#include "window/window.hpp"
#include "renderer/camera/scene_camera.hpp"

namespace acid
{

class Application;

/// Driven the core functions
class Engine
{
public:
    Engine();
    ~Engine();

    void Init();
    void Run();
private:
    bool running_ = true;

    Ref<SceneCamera> camera_;

    friend class Application;
};

} // namespace acid