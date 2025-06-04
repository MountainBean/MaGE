#ifndef GLM_TO_STRING_H
#define GLM_TO_STRING_H
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <ostream>

inline std::ostream& operator<< (std::ostream& out, const glm::vec3& glmVec) {
    out << glm::to_string(glmVec);
    return out;
}


#endif
