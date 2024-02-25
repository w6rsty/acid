#pragma once

#include "core/base.hpp"
#include "renderer/graphic_context.hpp"

#include <cstdint>
#include <string>

namespace acid
{

// 
struct WindowCreateInfo
{
    std::string Title = std::string("Untitled Window");
    uint32_t Width = 1280;
    uint32_t Height = 720;
    bool VSync = true;
};


/// Window agnostic need conditional compilation.
/// All implementations are defined in platform specific file
class Window final
{
public:
    struct Config
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool VSync;
    };

    Window(const WindowCreateInfo& info);
    ~Window();

    void OnUpdate();

    void SetSync(bool enable);

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    void* GetNativeWindow() const;

    static Ref<Window> Create(const WindowCreateInfo& info);
private:
    Config config_;
    /// TODO: Take this carefully.
    void* window_;
    Ref<GraphicContext> context_;
};

} // namespace acid