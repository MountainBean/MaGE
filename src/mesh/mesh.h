#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <material.h>

namespace sjd {

class Mesh {

protected:
    Mesh()
    :   m_model {1.0f}
    {
    }

public:

    virtual unsigned int defineVAOPointers() = 0;

    virtual void bufferData() = 0;

    void reset() {
        m_model = glm::mat4(1.0f);
    }

    void move(glm::vec3 location) {
        m_model = glm::translate(m_model, location);
    }

    void scale(glm::vec3 scaling) {
        m_model = glm::scale(m_model, scaling);
    }

    void rotateX(float radians) {
        m_model = glm::rotate(m_model, radians, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void rotateY(float radians) {
        m_model = glm::rotate(m_model, radians, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void rotateZ(float radians) {
        m_model = glm::rotate(m_model, radians, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    virtual void draw(glm::mat4 projection, glm::mat4 view, sjd::Shader& shader) = 0;

protected:
    unsigned int m_vao;
    unsigned int m_vbo;
    glm::mat4 m_model;
    sjd::Material m_material;
};
}
#endif
