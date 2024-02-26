#include "renderer/camera/scene_camera.hpp"

#include "core/assert.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

namespace acid
{


SceneCamera::SceneCamera()
{

}

SceneCamera::~SceneCamera()
{

}

const glm::mat4& SceneCamera::GetProjectionMatrix() const
{
    if (type_ == ProjectionType::Perspective)
    {
        return perspectiveProjection_;
    }
    else if (type_ == ProjectionType::Orthographic)
    {
        return orthographicProjection_;
    }
    else {
        AC_TAG_FATAL("Camera","Undefined projection type");
        return perspectiveProjection_;
    }
}

const glm::mat4& SceneCamera::GetViewMatrix() const
{
    return viewMatrix_;
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
{
    aspectRatio_ = static_cast<float>(width) / static_cast<float>(height);
}

void SceneCamera::SetPerspective(float aspect, float fov, float near, float far)
{
    type_ = ProjectionType::Perspective;
    aspectRatio_ = aspect;
    perspectiveVerticalFov_ = fov;
    perspectiveNear_ = near;
    perspectiveFar_ = far;
    CalculateProjection();
}

void SceneCamera::SetOrthographic(float aspect, float size, float near, float far)
{
    type_ = ProjectionType::Orthographic;
    aspectRatio_ = aspect;
    orthographicSize_ = size;
    orthographicNear_ = near;
    orthographicFear_ = far;
    CalculateProjection();
}

void SceneCamera::SetProjectionType(ProjectionType type)
{
    type_ = type;
    CalculateProjection();
}

void SceneCamera::CalculateProjection()
{   
    if (type_ == ProjectionType::Perspective)
    {
        perspectiveProjection_ = glm::perspective(
            glm::radians(perspectiveVerticalFov_),
            aspectRatio_,
            perspectiveNear_,
            perspectiveFar_);
    }
    else if (type_ == ProjectionType::Orthographic) 
    {
        orthographicProjection_ = glm::ortho(
            -aspectRatio_ * orthographicSize_ * 0.5f,
            aspectRatio_ * orthographicSize_ * 0.5f,
            -orthographicSize_ * 0.5f, orthographicSize_ * 0.5f,
            orthographicNear_, orthographicFear_);
    }
    else {
        AC_TAG_FATAL("Camera","Undefined projection type");
    }
}


void SceneCamera::CalculateView()
{
    glm::quat rotationQuat = glm::quat(glm::radians(rotation_));
    glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);

    viewMatrix_ = glm::inverse(
        glm::scale(glm::mat4(1.0f), scale_)
        * glm::translate(glm::mat4(1.0f), position_)
        * rotationMatrix
    );
}

/// Not often used in set the uniform
const glm::mat4& SceneCamera::GetViewProjectionMatrix()
{
    viewProjection_ = GetProjectionMatrix() * GetViewMatrix();

    return viewProjection_;
}

} // namespace acid