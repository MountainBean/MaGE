#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

namespace sjd {
struct Material {
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;
};

}
#endif
