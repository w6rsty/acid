#pragma once

#include "glm/glm.hpp"

namespace acid
{

struct PointLight
{                                   // base aligned
    alignas(16) glm::vec3 Position; // 16   0
    alignas(4)  float Constant;     // 4    16
    alignas(4)  float Linear;       // 4    20
    alignas(4)  float Quadratic;    // 4    24
    alignas(16) glm::vec3 Ambient;  // 16   32
    alignas(16) glm::vec3 Diffuse;  // 16   48
    alignas(16) glm::vec3 Specular; // 16   64
                                    // size 80
};

#define POINT_LIGHT_STD_140_SIZE 80

struct DirLight
{                                       // base aligned
    alignas(16) glm::vec3 Direction;    // 16   0
    alignas(16) glm::vec3 Ambient;      // 16   16
    alignas(16) glm::vec3 Diffuse;      // 16   32
    alignas(16) glm::vec3 Specular;     // 16   48
                                        // size 64
};

#define DIR_LIGHT_STD_140_SIZE 64

struct SpotLight
{                                       // base aligned
    alignas(16) glm::vec3 Position;     // 16   0
    alignas(16) glm::vec3 Direction;    // 16   16
    alignas(4)  float CutOff;           // 4    32
    alignas(4)  float OuterCutOff;      // 4    36
    alignas(4)  float Constant;         // 4    40
    alignas(4)  float Linear;           // 4    44
    alignas(4)  float Quadratic;        // 4    48
    alignas(16) glm::vec3 Ambient;      // 16   64
    alignas(16) glm::vec3 Diffuse;      // 16   80
    alignas(16) glm::vec3 Specular;     // 16   96
                                        // size 112
};

#define SPOT_LIGHT_STD_140_SIZE 112

} // namespace acid