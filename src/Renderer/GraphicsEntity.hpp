//
// Created by Lee on 2021/07/11.
//

#ifndef LEEFRAMEWORK_GRAPHICSENTITY_HPP
#define LEEFRAMEWORK_GRAPHICSENTITY_HPP
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


class PhysicsEntity;

struct GraphicsData{
    GLuint m_VBO;
    GLuint m_EBO;
    std::vector<glm::vec3>* m_position;
    std::vector<glm::uvec3>* m_indices;
    std::vector<glm::vec2>* m_uv;
    std::vector<glm::vec3>* m_normal;
    std::vector<glm::vec3>* m_color;
    glm::mat4 m_model_matrix;
    PhysicsEntity* m_mirror_pe;
    bool m_has_material{false};
    bool m_has_normal{false};
    bool m_has_texture{false};



    GLuint m_attrib_num;
//    void logGraphicsData() const{
//        printf("vertice number: %d\n",m_position->size());
//        printf("indices number: %d\n",m_indices->size());
//    };
};



#endif //LEEFRAMEWORK_GRAPHICSENTITY_HPP
