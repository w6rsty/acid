#pragma once

#include "core/base.hpp"
#include "core/assert.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "renderer/vertex.hpp"

#include "glm/glm.hpp"

#include <utility>
#include <vector>

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
    Quad,
    // 3D
    Box,
    Sphere,
    Tetrahedron,
    Pyramid,
    Cuboid,
    Capsule
};

struct GeoData
{
    std::vector<Vertex> Vertices;
    size_t VertexSize;
    std::vector<uint32_t> Indices;
    size_t IndexCount;
};

struct Point
{
    glm::vec3 Position;
};

struct Line
{
    glm::vec3 Start;
    glm::vec3 End;
};

// 2D ===============

struct Rect
{
    glm::vec3 Origin = {0.0f, 0.0f, 0.0f};
    float Width;
    float Height;
};

struct Circle
{
    glm::vec3 Center = {0.0f, 0.0f, 0.0f};
    float Radius = 0.5f;

    static GeoData UnitData(uint32_t precision = 24)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve(3 * (precision + 1) + 1);
        indices.reserve(3 * precision + 1);
        vertices.push_back({ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f } });
        for (uint32_t i = 0; i <= precision; i++)
        {
            Vertex vertex;
            // use glm pi
            float angle = 2 * glm::two_pi<float>() * i / precision;
            glm::vec3 position = { 0.5f * glm::cos(angle), 0.5f * glm::sin(angle), 0.0f };
            vertex.Position = position;
            vertex.Normal = { 0.0f, 0.0f, 1.0f };
            vertex.TexCoord = { 0.5f + 0.5f * glm::cos(angle), 0.5f + 0.5f * glm::sin(angle) };
            vertices.push_back(vertex);

            if (i < precision)
            {
                indices.push_back(0);
                indices.push_back(i + 1);
                indices.push_back(i + 2);
            }
        }
        return {
            std::move(vertices),
            3 * (precision + 1) * sizeof(Vertex),
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
            {
                Vertex { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
                Vertex { { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
                Vertex { { 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f} }
            },
            3 * sizeof(Vertex),
            { 0, 1, 2 },
            3
        };
    
    }
};

struct Quad
{
    glm::vec3 Origin = {0.0f, 0.0f, 0.0f};
    float Width = 1.0f;

    static GeoData UnitData()
    {
        return {
            {
               Vertex { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
               Vertex { { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
               Vertex { { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
               Vertex { {-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }
            },
            4 * sizeof(Vertex),
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
};

struct Sphere
{
    glm::vec3 Center = {0.0f, 0.0f, 0.0f};
    float Radius = 0.5f;

    static GeoData UnitData(uint32_t precision = 24)
    {
        // use glm math
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve((precision + 1) * (precision + 1));
        indices.reserve(6 * precision * precision);
        for (uint32_t i = 0; i <= precision; i++)
        {
            for (uint32_t j = 0; j <= precision; j++)
            {
                Vertex vertex;
                float y = glm::cos(glm::pi<float>() * i / precision);
                float x = glm::sin(glm::pi<float>() * i / precision) * glm::cos(2 * glm::pi<float>() * j / precision);
                float z = glm::sin(glm::pi<float>() * i / precision) * glm::sin(2 * glm::pi<float>() * j / precision);
                vertex.Position = { 0.5f * x, 0.5f * y, 0.5f * z };
                vertex.Normal = { x, y, z };
                vertex.TexCoord = { (float)j / precision, (float)i / precision};
                vertices[precision * i + j] = vertex;
            }
        }
        for (uint32_t i = 0; i < precision; i++)
        {
            for (uint32_t j = 0; j < precision; j++)
            {
                indices.push_back(precision * i + j);
                indices.push_back(precision * (i + 1) + j);
                indices.push_back(precision * i + j + 1);
                indices.push_back(precision * i + j + 1);
                indices.push_back(precision * (i + 1) + j);
                indices.push_back(precision * (i + 1) + j + 1);
            }
        }
        return {
            std::move(vertices),
            (precision + 1) * (precision + 1) * sizeof(Vertex),
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
    glm::vec3 Origin = {0.0f, 0.0f, 0.0f};
    float Width = 1.0f;

    static GeoData UnitData()
    {
        return {
            {
                Vertex { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
                Vertex { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
                Vertex { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
                Vertex { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },
                Vertex { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f} },
                Vertex { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f} },
                Vertex { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f} },
                Vertex { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f} }
            },
            8 * sizeof(Vertex),
            { 0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 5, 4, 7, 7, 6, 5, 4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 0, 4, 5, 5, 1, 0 },
            36
        };
    }
};

struct Capsule
{
    glm::vec3 A;
    glm::vec3 B;
    float Radius;

    static GeoData UnitData(float radius = 0.5f, float height = 0.5f, uint32_t precision = 24)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve((precision + 1) * (precision + 1));
        indices.reserve(6 * precision * precision);
        AC_LOG_TRACE(vertices.capacity(), " ", vertices.size());
        for (uint32_t i = 0; i <= precision; i++)
        {
            for (uint32_t j = 0; j <= precision; j++)
            {
                Vertex vertex;
                float y = glm::cos(glm::pi<float>() * i / precision);
                float x = glm::sin(glm::pi<float>() * i / precision) * glm::cos(2 * glm::pi<float>() * j / precision);
                float z = glm::sin(glm::pi<float>() * i / precision) * glm::sin(2 * glm::pi<float>() * j / precision);
                if (y < 0.0f)
                {
                    vertex.Position = { radius * x, radius * y - height / 2.0f, radius * z };
                    vertex.Normal = { x, y, z};
                }
                else
                {
                    vertex.Position = { radius * x, radius * y + height / 2.0f, radius * z };
                    vertex.Normal = { x, y, z };
                }
                vertex.TexCoord = { (float)j / precision, (float)i / precision};
                vertices[precision * i + j] = vertex;
            }
        }
        for (uint32_t i = 0; i < precision; i++)
        {
            for (uint32_t j = 0; j < precision; j++)
            {
                indices.push_back(precision * i + j);
                indices.push_back(precision * (i + 1) + j);
                indices.push_back(precision * i + j + 1);
                indices.push_back(precision * i + j + 1);
                indices.push_back(precision * (i + 1) + j);
                indices.push_back(precision * (i + 1) + j + 1);
            }
        }
        AC_LOG_TRACE(vertices.capacity(), " ", vertices.size());
        return {
            std::move(vertices),
            2 * (precision + 1) * (precision + 1) * sizeof(Vertex),
            std::move(indices),
            6 * precision * precision
        };
    }
};

} // namespace acid::geo