#pragma once

#include "core/base.hpp"
#include "renderer/renderer_enum.hpp"

#include <cstdint>
#include <string>

namespace acid
{

class Texture
{
public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(void* data, uint32_t size) = 0;
    virtual uint32_t GetRendererID() const = 0;

    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    static Ref<Texture> Create(uint32_t width, uint32_t height, AC_TEXTURE_WARP_MODE mode = AC_TEXTURE_WARP_MODE::REPEAT);
    static Ref<Texture> Create(const std::string& path, AC_TEXTURE_WARP_MODE mode = AC_TEXTURE_WARP_MODE::REPEAT);
};

} // namespace acid