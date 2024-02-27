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
    Quad,
    Triangle,
    Circle,
    Pyramid,
    Cuboid,
    Cone,
    Sphere,
    Capsule
};

struct GeoData
{
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;
};

// 3D ===============

struct Quad
{
    static GeoData UnitData()
    {
        return {
            {
               Vertex { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
               Vertex { { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
               Vertex { { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
               Vertex { {-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }
            },
            { 0, 1, 2, 2, 3, 0 },
        };
    }

    static GeoData Data;
};

struct Triangle
{
    static GeoData UnitData()
    {
        return {
            {
                Vertex { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
                Vertex { { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
                Vertex { { 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f} }
            },
            { 0, 1, 2 },
        };
    }

    static GeoData Data;
};

struct Circle
{
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
            std::move(indices),
        };
    }

    static GeoData Data;
};

struct Cuboid
{
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
            { 0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 5, 4, 7, 7, 6, 5, 4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 0, 4, 5, 5, 1, 0 },
        };
    }

    static GeoData Data;
};

struct Pyramid
{
    static GeoData UnitData()
    {
        return {
            {
                Vertex { { 0.0f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f} },
                Vertex { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
                Vertex { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
                Vertex { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
                Vertex { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }
            },
            { 0, 1, 2, 2, 3, 0, 0, 3, 4, 4, 1, 0, 1, 2, 4, 4, 3, 2 },
        };
    }

    static GeoData Data;
};

struct Cone
{
    static GeoData UnitData(uint32_t precision = 24)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve((precision + 1) * 2);
        indices.reserve(3 * precision);
        vertices.push_back({ { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.5f, 0.5f } });
        for (uint32_t i = 0; i <= precision; i++)
        {
            Vertex vertex;
            float angle = 2 * glm::pi<float>() * i / precision;
            vertex.Position = { 0.5f * glm::cos(angle), -0.5f, 0.5f * glm::sin(angle) };
            vertex.Normal = glm::normalize(glm::vec3(vertex.Position.x, 0.0f, vertex.Position.z));
            vertex.TexCoord = { 0.5f + 0.5f * glm::cos(angle), 0.5f + 0.5f * glm::sin(angle) };
            vertices.push_back(vertex);
            if (i < precision)
            {
                indices.push_back(0);
                indices.push_back(i + 1);
                indices.push_back(i + 2);
            }
        }
        for (uint32_t i = 0; i < precision; i++)
        {
            indices.push_back(1);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }
        return {
            std::move(vertices),
            std::move(indices),
        };
    }  

    static GeoData Data;
};

struct Sphere
{
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
            std::move(indices),
        };
    }

    static GeoData Data;
};

struct Capsule
{
    static GeoData UnitData(float radius = 0.5f, float height = 0.5f, uint32_t precision = 24)
    {
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
        return {
            std::move(vertices),
            std::move(indices),
        };
    }

    static GeoData Data;
};


// Simple geo support batch rendering
inline GeoData Quad::Data = Quad::UnitData();
inline GeoData Triangle::Data = Triangle::UnitData();
inline GeoData Cuboid::Data = Cuboid::UnitData();
inline GeoData Pyramid::Data = Pyramid::UnitData();

inline GeoData Circle::Data = Circle::UnitData(64);
inline GeoData Cone::Data = Cone::UnitData(64);
inline GeoData Sphere::Data = Sphere::UnitData(64);
inline GeoData Capsule::Data = Capsule::UnitData(0.5f, 0.5f, 64);

} // namespace acid::geo