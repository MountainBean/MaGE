#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sjd {

class Camera {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Camera(glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 initialFocus = glm::vec3(0.0f));

    const glm::vec3& getPos() { return m_pos; };

    glm::mat4 getViewMatrix(){
        return glm::lookAt(m_pos, m_pos + m_front, m_up);
    }

    void processMovement(Movement direction, float deltaTime);

    void turnTo(glm::vec3 point3d = glm::vec3(0.0f));

    void processXYRotation(float xoffset,
                           float yoffset, 
                           GLboolean constrainPitch = true);

    void processChangeZoom(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void _updateCameraVectors();


    glm::vec3 m_pos;
    glm::vec3 m_up        {0.0f, 1.0f, 0.0f};
    glm::vec3 m_worldUp   {m_up};
    glm::vec3 m_front     {glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 m_right     {};
    float m_pitch     {0.0f};
    float m_yaw       {-90.0f};
    float m_movementSpeed {2.5f};
    float m_mouseSensitivity {0.1f};
    float m_zoom {45.0f};

};
}
#endif
