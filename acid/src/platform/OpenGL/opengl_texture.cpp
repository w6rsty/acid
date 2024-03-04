#include "platform/OpenGL/opengl_texture.hpp"

#include "core/assert.hpp"

#include "glad/glad.h"
#include "stb_image.h"

#include <string>

namespace acid
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, AC_TEXTURE_WARP_MODE mode)
{
    glGenTextures(1, &rendererID_);
    glBindTexture(GL_TEXTURE_2D, rendererID_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, AC_TEXTURE_WARP_MODE_TO_GL(mode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, AC_TEXTURE_WARP_MODE_TO_GL(mode));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);

    width_ = width;
    height_ = height;
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path, AC_TEXTURE_WARP_MODE mode)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    AC_ASSERT_MSG(data, "Failed to load image!");

    width_ = width;
    height_ = height;

    glGenTextures(1, &rendererID_);
    glBindTexture(GL_TEXTURE_2D, rendererID_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, AC_TEXTURE_WARP_MODE_TO_GL(mode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, AC_TEXTURE_WARP_MODE_TO_GL(mode));

    GLenum format = 0;
    if (channels == 4)
    {
        format = GL_RGBA;
    }
    else if (channels == 3)
    {
        format = GL_RGB;
    }
    else
    {
        AC_ASSERT_MSG(false, "Unknown format!");
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &rendererID_);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, rendererID_);
}

void OpenGLTexture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size)
{
    uint32_t bpp = 4;
    AC_ASSERT_MSG(size == width_ * height_ * bpp, "Data must be entire texture!");
    glBindTexture(GL_TEXTURE_2D, rendererID_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace acid
