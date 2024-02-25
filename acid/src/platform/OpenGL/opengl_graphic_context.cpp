#include "platform/OpenGL/opengl_graphic_context.hpp"

#include "core/assert.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace acid
{

OpenGLGraphicContext::OpenGLGraphicContext(GLFWwindow* handle)
: handle_(handle)
{

}

void OpenGLGraphicContext::Init()
{
    glfwMakeContextCurrent(handle_);
    int GLADInitStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    AC_ASSERT_MSG(GLADInitStatus, "Failed to initialize GLAD!");

    AC_TAG_INFO("Vendor", glGetString(GL_VENDOR));
    AC_TAG_INFO("Device", glGetString(GL_RENDERER));
    AC_TAG_INFO("GLVersion", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
}

void OpenGLGraphicContext::Swap()
{
    glfwSwapBuffers(handle_);
}

} // namespace acid