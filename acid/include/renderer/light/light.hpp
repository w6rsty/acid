#pragma once

#include "glm/glm.hpp"

namespace acid
{

struct Light
{
    glm::vec3 Position;

    glm::vec3 Ambient;
    float AmbientIntensity;
    
    glm::vec3 Diffuse;
    float DiffuseIntensity;

    glm::vec3 Specular;
    float SpecularIntensity;
    float Shininess;
};

} // namespace acid