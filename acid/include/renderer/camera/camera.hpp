#pragma once

#include "core/base.hpp"

#include "glm/glm.hpp"

namespace acid
{

enum class ProjectionType
{
    None = -1,
    Orthographic = 0,
    Perspective = 1,
};

/// Camera base class only provides projection matrix.
/// derived classes should provide view matrix.
class Camera 
{
public:
    virtual ~Camera() = default;

    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;
};


} // namespace acid