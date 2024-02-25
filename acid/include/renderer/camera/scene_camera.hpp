#pragma once

#include "renderer/camera/camera.hpp"

#include "glm/glm.hpp"

namespace acid
{

class SceneCamera : public Camera
{
public:
    SceneCamera();
    virtual ~SceneCamera() override;

    // Projection
    virtual const glm::mat4& GetProjectionMatrix() const override;
    virtual void SetViewportSize(uint32_t width, uint32_t height) override;

    void SetPerspective(float aspect, float fov, float near, float far);
    void SetOrthographic(float aspect, float size, float near, float far);
    void SetProjectionType(ProjectionType type);
    // View
    const glm::mat4& GetViewMatrix() const;

    const glm::vec3& GetPosition() const { return position_; }
    void SetPosition(const glm::vec3& position) { position_ = position; CalculateView(); }
    const glm::vec3& GetRotation() const { return rotation_; }
    void SetRotation(const glm::vec3& rotation) { rotation_ = rotation; CalculateView(); }
    const glm::vec3& GetScale() const { return scale_; }
    void SetScale(const glm::vec3& scale) { scale_ = scale; CalculateView(); }

    const glm::mat4& GetViewProjectionMatrix();
private:
    void CalculateProjection();
    void CalculateView();
private:
    ProjectionType type_ = ProjectionType::None;
    // Projection
    float aspectRatio_ = 0.0f;

    float perspectiveVerticalFov_ = 45.0f;
    float perspectiveNear_ = 0.01f;
    float perspectiveFar_ = 1000.0f;
    glm::mat4 perspectiveProjection_ = glm::mat4(1.0f);

    float orthographicSize_ = 1.0f;
    float orthographicNear_ = -1.0f;
    float orthographicFear_ = 1.0f;
    glm::mat4 orthographicProjection_ = glm::mat4(1.0f);

    glm::mat4 viewProjection_ = glm::mat4(1.0f);
    // View
    glm::mat4 viewMatrix_ = glm::mat4(1.0f);

    glm::vec3 position_ = glm::vec3(0.0f);
    glm::vec3 rotation_ = glm::vec3(0.0f);
    glm::vec3 scale_ = glm::vec3(1.0f);
};

} // namespace acid