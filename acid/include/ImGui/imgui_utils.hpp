#pragma once

namespace acid
{

class ImGuiLayer
{
public:
    static void Init();
    static void Shutdown();

    static void Begin();
    static void End();
};

}; // namespace acid