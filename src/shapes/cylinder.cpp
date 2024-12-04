#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}


void Cylinder::setVertexData() {
    float angleStep = glm::radians(360.f / m_param2);
    glm::vec3 bottomCenter(0.f, -0.5f, 0.f);
    glm::vec3 topCenter(0.f, 0.5f, 0.f);

    for (int i = 0; i < m_param1; ++i) {
        for (int j = 0; j < m_param2; ++j) {
            float angle = j * angleStep;

            glm::vec3 bottomBottomLeft(cos(angle) * 0.5f, -0.5f, sin(angle) * 0.5f);
            glm::vec3 bottomBottomRight(cos(angle + angleStep) * 0.5f, -0.5f, sin(angle + angleStep) * 0.5f);
            glm::vec3 bottomTopLeft(cos(angle) * (0.5f / m_param1) * i, -0.5f, sin(angle) * (0.5f / m_param1) * i);
            glm::vec3 bottomTopRight(cos(angle + angleStep) * (0.5f / m_param1) * i, -0.5f, sin(angle + angleStep) * (0.5f / m_param1) * i);

            glm::vec3 topBottomLeft(cos(angle) * 0.5f, 0.5f, sin(angle) * 0.5f);
            glm::vec3 topBottomRight(cos(angle + angleStep) * 0.5f, 0.5f, sin(angle + angleStep) * 0.5f);
            glm::vec3 topTopLeft(cos(angle) * (0.5f / m_param1) * i, 0.5f, sin(angle) * (0.5f / m_param1) * i);
            glm::vec3 topTopRight(cos(angle + angleStep) * (0.5f / m_param1) * i, 0.5f, sin(angle + angleStep) * (0.5f / m_param1) * i);

            insertVec3(m_vertexData, bottomBottomLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, bottomBottomRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, bottomTopLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));

            insertVec3(m_vertexData, bottomTopRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, bottomTopLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, bottomBottomRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));

            insertVec3(m_vertexData, topBottomLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
            insertVec3(m_vertexData, topTopLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
            insertVec3(m_vertexData, topBottomRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, 1.f, 0.f)));

            insertVec3(m_vertexData, topTopRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
            insertVec3(m_vertexData, topBottomRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
            insertVec3(m_vertexData, topTopLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
        }
    }


    for (int i = 0; i < m_param1; ++i) {
        float bottomY = -0.5f + (i / float(m_param1));
        float topY = -0.5f + ((i + 1.f) / m_param1);

        for (int j = 0; j < m_param2; ++j) {
            float angle = j * angleStep;

            glm::vec3 bottomLeft(cos(angle) * 0.5f, bottomY, sin(angle) * 0.5f);
            glm::vec3 bottomRight(cos(angle + angleStep) * 0.5f, bottomY, sin(angle + angleStep) * 0.5f);
            glm::vec3 topLeft(cos(angle) * 0.5f, topY, sin(angle) * 0.5f);
            glm::vec3 topRight(cos(angle + angleStep) * 0.5f, topY, sin(angle + angleStep) * 0.5f);

            glm::vec3 bottomLeftNormal = glm::normalize(glm::vec4(bottomLeft[0], 0.f, bottomLeft[2], 0.f));
            glm::vec3 bottomRightNormal = glm::normalize(glm::vec4(bottomRight[0], 0.f, bottomRight[2], 0.f));
            glm::vec3 topLeftNormal = glm::normalize(glm::vec4(topLeft[0], 0.f, topLeft[2], 0.f));
            glm::vec3 topRightNormal = glm::normalize(glm::vec4(topRight[0], 0.f, topRight[2], 0.f));

            insertVec3(m_vertexData, bottomLeft);
            insertVec3(m_vertexData, bottomLeftNormal);
            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, topLeftNormal);
            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, bottomRightNormal);

            insertVec3(m_vertexData, topRight);
            insertVec3(m_vertexData, topRightNormal);
            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, bottomRightNormal);
            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, topLeftNormal);
        }
    }
}

void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Cylinder::clearData() {
    m_vertexData = std::vector<float>();
}
