#include <camera.h>

namespace sjd {

Camera::Camera(glm::vec3 initialPosition,
               glm::vec3 initialFocus)
:   m_pos {initialPosition}
{
    _updateCameraVectors();
    turnTo(initialFocus);
}

void Camera::processMovement(Camera::Movement direction,
                                    float deltaTime) {
    float velocity {m_movementSpeed * deltaTime};
    if (direction == FORWARD)
        m_pos += glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z)) * velocity;
    if (direction == BACKWARD)
        m_pos -= glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z)) * velocity;
    if (direction == LEFT)
        m_pos -= m_right * velocity;
    if (direction == RIGHT)
        m_pos += m_right * velocity;
    if (direction == UP)
        m_pos += m_worldUp * velocity;
    if (direction == DOWN)
        m_pos -= m_worldUp * velocity;
}

void Camera::turnTo(glm::vec3 point3d) {
    float offset_x = m_pos.x - point3d.x;
    float offset_y = m_pos.y - point3d.y;
    float offset_z = m_pos.z - point3d.z;
    float offset_xz = glm::length(glm::vec2(offset_x, offset_z));

    if (offset_x == 0) {
        (offset_z > 0) ? m_yaw = -90.0f : m_yaw = 90.0f;
    }
    if (offset_x < 0) {
        m_yaw = atanf(offset_z/offset_x) * 180/glm::pi<float>();
    }
    else if (offset_x > 0) {
        if (offset_z < 0)
            m_yaw = atanf(offset_z/offset_x) * 180/glm::pi<float>() + 180.0f;
        else
            m_yaw = atanf(offset_z/offset_x) * 180/glm::pi<float>() - 180.0f;
    }
    m_pitch = atanf(-offset_y/offset_xz) * 180/glm::pi<float>();

    _updateCameraVectors();
}

void Camera::processXYRotation(float xoffset,
                               float yoffset,
                               GLboolean constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // update m_front, right and Up Vectors using the updated Euler angles
    _updateCameraVectors();
}

void Camera::processChangeZoom(float yoffset) {
    m_zoom -= (float)yoffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void Camera::_updateCameraVectors() {
    // calculate the new front vector
    glm::vec3 newfront {
        cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        sin(glm::radians(m_pitch)),
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
    };
    m_front = glm::normalize(newfront);
    // also re-calculate the right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

}
