#pragma once

#include "window/window.hpp"
#include "renderer/vertex_array.hpp"

namespace acid
{

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
    Ref<VertexArray> vertexArray_;
    Ref<Window> window_;
};

} // namespace acid