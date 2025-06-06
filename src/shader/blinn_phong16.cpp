#include <Shader.h>
#include <iostream>

namespace sjd {

class BlinnPhong16 : Shader {
public:
    BlinnPhong16(const std::string& vertexPath,
                 const std::string& fragmentPath)
    : Shader {vertexPath, fragmentPath}
    {
        m_vertexAttributes = 3;
    }

    void projection(const glm::mat4& pMat){m_projection = &pMat;};
    auto projection() const -> const glm::mat4& {return *m_projection;};
    void model(const glm::mat4& mMat){m_model = &mMat;};
    auto model() const -> const glm::mat4& {return *m_model;};
    void view(const glm::mat4& vMat){m_view = &vMat;};
    auto view() const -> const glm::mat4& {return *m_view;};

    void draw(uint16_t triangles) {
        if (!m_projection) {
            std::cout << "ERROR::SHADER::PROJECTION::NOT_SET\n";
            return;
        }
        if (!m_model) {
            std::cout << "ERROR::SHADER::MODEL::NOT_SET\n";
            return;
        }
        if (!m_view) {
            std::cout << "ERROR::SHADER::VIEW::NOT_SET\n";
            return;
        }
        use();
        setUniform<const glm::mat4&>("projection", *m_projection);
        setUniform<const glm::mat4&>("model", *m_model);
        setUniform<const glm::mat4&>("view", *m_view);
        glDrawArrays(GL_TRIANGLES, 0, triangles);
    }

private:
    const glm::mat4* m_projection {nullptr};
    const glm::mat4* m_model {nullptr};
    const glm::mat4* m_view {nullptr};
};

}
