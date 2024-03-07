#include "renderer/light/material.hpp"


namespace acid
{

Material::Material()
{
    ambientMap_ = Texture2D::Create("assets/textures/Checkerboard.png");
    diffuseMap_ = Texture2D::Create("assets/textures/Checkerboard.png");
    specularMap_ = Texture2D::Create("assets/textures/Checkerboard.png");
}

} // namespace acid