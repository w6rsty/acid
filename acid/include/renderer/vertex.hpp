#pragma once

#include "glm/glm.hpp"

namespace acid
{

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;

    Vertex(const glm::vec3 &position = { 0.0f, 0.0f, 0.0f }, const glm::vec3 &normal = { 0.0f, 0.0f, 0.0f }, const glm::vec2 &texCoord = { 0.0f, 0.0f })
    : Position(position), Normal(normal), TexCoord(texCoord)
    {}
};

} // namespace acid