#pragma once

#include "renderer/texture.hpp"

namespace acid
{

class OpenGLTexture2D final : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height, AC_TEXTURE_WARP_MODE mode);
    OpenGLTexture2D(const std::string& path, AC_TEXTURE_WARP_MODE mode);
    virtual ~OpenGLTexture2D() override;

    virtual uint32_t GetWidth() const override { return width_; }
    virtual uint32_t GetHeight() const override { return height_; }

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;

    virtual void SetData(void* data, uint32_t size) override;

    virtual bool operator==(const Texture& other) const override
    {
        return rendererID_ == static_cast<const OpenGLTexture2D&>(other).rendererID_;
    }
private:
    uint32_t rendererID_;
    uint32_t width_, height_;
};

} // namespace acid