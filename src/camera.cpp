#include "camera.h"

Camera::Camera(SceneCameraData cameraData) : m_cameraData(cameraData) {
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 translate = {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        -m_cameraData.pos[0], -m_cameraData.pos[1], -m_cameraData.pos[2], 1.f

    };

    glm::vec3 w = -m_cameraData.look;
    w = glm::normalize(w);
    glm::vec3 upVec = glm::vec3(m_cameraData.up);
    glm::vec3 v = upVec - glm::dot(upVec, w) * w;
    v = glm::normalize(v);
    glm::vec3 u = cross(v, w);

    glm::mat4 rotate {
        u[0], v[0], w[0], 0.0f,
        u[1], v[1], w[1], 0.0f,
        u[2], v[2], w[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    return rotate * translate;
}

float Camera::getAspectRatio() const {
    return 0.f;
}

float Camera::getHeightAngle() const {
    return m_cameraData.heightAngle;
}

float Camera::getFocalLength() const {
    return m_cameraData.focalLength;
}

float Camera::getAperture() const {
    return m_cameraData.aperture;
}
