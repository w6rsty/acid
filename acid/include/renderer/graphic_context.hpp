#pragma once

#include "core/base.hpp"

struct GLFWwindow;

namespace acid
{

class GraphicContext
{
public:
    virtual void Init() = 0;
    virtual void Swap() = 0;

    static Ref<GraphicContext> Create(GLFWwindow* handle);
};

} // namespace acid