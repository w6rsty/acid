#pragma once

namespace acid
{

enum class AcidGraphicAPI
{
    None = 0,
    OpenGL = 1,
};

const static AcidGraphicAPI AcidGraphicAPI = AcidGraphicAPI::OpenGL;

} // namespace acid