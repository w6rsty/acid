#pragma once

#include "glm/glm.hpp"

namespace acid
{

struct Cuboid
{   
    static glm::vec3 Vertices[24];
    static glm::vec3 Normals[6];
    static glm::vec2 TexCoords[24];
};

inline glm::vec3 Cuboid::Vertices[24] = {
        // front
    { -0.125f, -0.125f,  0.125f },
    {  0.125f, -0.125f,  0.125f },
    {  0.125f,  0.125f,  0.125f },
    { -0.125f,  0.125f,  0.125f },
    // back
    { -0.125f, -0.125f, -0.125f },
    {  0.125f, -0.125f, -0.125f },
    {  0.125f,  0.125f, -0.125f },
    { -0.125f,  0.125f, -0.125f },
    // left
    { -0.125f, -0.125f, -0.125f },
    { -0.125f, -0.125f,  0.125f },
    { -0.125f,  0.125f,  0.125f },
    { -0.125f,  0.125f, -0.125f },
    // right
    {  0.125f, -0.125f,  0.125f },
    {  0.125f, -0.125f, -0.125f },
    {  0.125f,  0.125f, -0.125f },
    {  0.125f,  0.125f,  0.125f },
    // top
    { -0.125f,  0.125f,  0.125f },
    {  0.125f,  0.125f,  0.125f },
    {  0.125f,  0.125f, -0.125f },
    { -0.125f,  0.125f, -0.125f },
    // bottom
    { -0.125f, -0.125f, -0.125f },
    {  0.125f, -0.125f, -0.125f },
    {  0.125f, -0.125f,  0.125f },
    { -0.125f, -0.125f,  0.125f },
};

inline glm::vec3 Cuboid::Normals[6] = {
    // front
    {  0.0f,  0.0f,  1.0f },
    // back
    {  0.0f,  0.0f, -1.0f },
    // left
    { -1.0f,  0.0f,  0.0f },
    // right
    {  1.0f,  0.0f,  0.0f },
    // top
    {  0.0f,  1.0f,  0.0f },
    // bottom
    {  0.0f, -1.0f,  0.0f },
};

inline glm::vec2 Cuboid::TexCoords[24] = {
    // front
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    // back
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    // left
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    // right
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    // top
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    // bottom
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
};

} // namespace acid