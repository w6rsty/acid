#pragma once

#include "glm/glm.hpp"

namespace acid
{

struct Vertex
{
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Normal = { 0.0f, 0.0f, 0.0f };
    glm::vec2 TexCoord = { 0.0f, 0.0f };
};

} // namespace acid