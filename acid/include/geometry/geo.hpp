#pragma once

#include "core/base.hpp"
#include "core/assert.hpp"

#include "glm/glm.hpp"

#include <utility>
#include <vector>
#include <cmath>

namespace acid::geo
{

enum class GeoType
{
    None,
    Point,
    Line,
    // 2D
    Rect,
    Circle,
    Triangle,
    Polygon,
    Quad,
    // 3D
    Box,
    Sphere,
    Tetrahedron,
    Pyramid,
    Cuboid,
    Cylinder,
    Cone,
    Capsule
};

struct GeoData
{
    std::vector<float> Vertices;
    size_t VertexSize;
    std::vector<uint32_t> Indices;
    size_t IndexCount;
};

struct Point
{
    glm::vec3 Position;

    static GeoData UnitData()
    {
        return {
            { 0.0f, 0.0f, 0.0f },
            3 * sizeof(float),
            {},
            0
        };
    }
};

struct Line
{
    glm::vec3 Start;
    glm::vec3 End;

    static GeoData UnitData()
    {
        return {
            { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
            6 * sizeof(float),
            { 0, 1 },
            2
        };
    
    }
};

// 2D ===============

struct Rect
{
    glm::vec3 Origin;
    float Width;
    float Height;

    static GeoData UnitData()
    {
        return {
            { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f },
            12 * sizeof(float),
            { 0, 1, 2, 2, 3, 0 },
            6
        };
    
    }
};

struct Circle
{
    glm::vec3 Center;
    float Radius;
    float precision;

    static GeoData UnitData(uint32_t precision = 24)
    {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve(3 * (precision + 1));
        indices.reserve(3 * precision);
        for (uint32_t i = 0; i <= precision; i++)
        {
            float angle = 2 * M_PI * i / precision;
            vertices.push_back(0.5 * std::cos(angle));
            vertices.push_back(0.5 * std::sin(angle));
            vertices.push_back(0.0f);
            if (i > 0)
            {
                indices.push_back(0);
                indices.push_back(i);
                indices.push_back(i - 1);
            }
        }
        indices.push_back(0);
        indices.push_back(precision);
        indices.push_back(1);
        return {
            std::move(vertices),
            3 * (precision + 1) * sizeof(float),
            std::move(indices),
            3 * precision
        };
    }
};

struct Triangle
{
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;

    static GeoData UnitData()
    {
        return {
            { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f },
            9 * sizeof(float),
            { 0, 1, 2 },
            3
        };
    
    }
};

struct Polygon
{
    std::vector<glm::vec3> Points;
};

struct Quad
{
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 D;

    static GeoData UnitData()
    {
        return {
            { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f },
            12 * sizeof(float),
            { 0, 1, 2, 2, 3, 0 },
            6
        };
    }     
};

// 3D ===============

struct Box
{
    glm::vec3 Origin;
    float Width;
    float Height;
    float Depth;

    static GeoData UnitData()
    {
        return {
            {
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f
            },
            24 * sizeof(float),
            {
                0, 1, 2, 2, 3, 0,
                1, 5, 6, 6, 2, 1,
                5, 4, 7, 7, 6, 5,
                4, 0, 3, 3, 7, 4,
                3, 2, 6, 6, 7, 3,
                4, 5, 1, 1, 0, 4
            },
            36
        };
    }
};

struct Sphere
{
    glm::vec3 Center;
    float Radius;
    float precision;

    static GeoData UnitData(uint32_t precision = 24)
    {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve(3 * (precision + 1) * (precision + 1));
        indices.reserve(6 * precision * precision);
        for (uint32_t i = 0; i <= precision; i++)
        {
            float theta = M_PI * i / precision;
            for (uint32_t j = 0; j <= precision; j++)
            {
                float phi = 2 * M_PI * j / precision;
                vertices.push_back(0.5 * std::sin(theta) * std::cos(phi));
                vertices.push_back(0.5 * std::sin(theta) * std::sin(phi));
                vertices.push_back(0.5 * std::cos(theta));
                if (i > 0 && j > 0)
                {
                    uint32_t a = (precision + 1) * i + j;
                    uint32_t b = (precision + 1) * i + j - 1;
                    uint32_t c = (precision + 1) * (i - 1) + j - 1;
                    uint32_t d = (precision + 1) * (i - 1) + j;
                    if (i < precision)
                    {
                        indices.push_back(a);
                        indices.push_back(b);
                        indices.push_back(c);
                    }
                    if (i > 1)
                    {
                        indices.push_back(a);
                        indices.push_back(c);
                        indices.push_back(d);
                    }
                }
            }
        }
        return {
            std::move(vertices),
            3 * (precision + 1) * (precision + 1) * sizeof(float),
            std::move(indices),
            6 * precision * precision
        };
    
    }
};

struct Tetrahedron
{
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 D;
};

struct Pyramid
{
    glm::vec3 Apex;
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 D;
};

struct Cuboid
{
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 D;
    glm::vec3 E;
    glm::vec3 F;
    glm::vec3 G;
    glm::vec3 H;
};

struct Cylinder
{
    glm::vec3 Center;
    float Radius;
    float Height;
    float precision;
};

struct Cone
{
    glm::vec3 Apex;
    glm::vec3 Center;
    float Radius;
};

struct Capsule
{
    glm::vec3 A;
    glm::vec3 B;
    float Radius;
    float precision;
};

} // namespace acid::geo