#pragma once

#include <memory>

namespace acid
{

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

struct RendererStats
{
    uint32_t DrawCalls = 0;
    uint32_t VoxelCount = 0;
    uint32_t VertexCount = 0;
    uint32_t TriangleCount = 0;

    uint32_t GetTotalVertexCount() const { return VertexCount; }
    uint32_t GetTotalTriangleCount() const { return TriangleCount; }
    uint32_t GetTotalDrawCalls() const { return DrawCalls; }
    void Reset()
    {
        DrawCalls = 0;
        VertexCount = 0;
        TriangleCount = 0;
        VoxelCount = 0;
    }
};

}