#pragma once

#include "renderer/texture.hpp"

namespace acid
{

class Material
{
public:
    Material();
// private:
    Ref<Texture> ambientMap_ = nullptr;
    Ref<Texture> diffuseMap_ = nullptr;
    Ref<Texture> specularMap_ = nullptr;
};

} // namespace acid