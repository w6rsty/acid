#pragma once

#include "core/base.hpp"
#include "engine/engine.hpp"
#include "window/window.hpp"

namespace acid
{

class Application
{
public:
    Application();
    ~Application();

    void Init();

    void Run();
    
    const Ref<Window> GetWindow() const { return window_; }

    static Application& Instance() { return *sInstance_; }
private:
    static Application* sInstance_;
    Ref<Window> window_;
    Ref<Engine> engine_;
};

} // namespace acid 