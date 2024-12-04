#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}


void Cone::setVertexData() {
    // TODO for Project 5: Lights, Camera
    glm::vec3 tip(0.f, 0.5f, 0.f);
    glm::vec3 baseCenter(0.f, -0.5f, 0.f);
    glm::vec3 baseNormal = glm::vec3(0.f, -1.f, 0.f);
    float angleStep = glm::radians(360.f / m_param2);

    for (int i = 0; i < m_param1; ++i) {
        float bottomY = -0.5f + (i / float(m_param1));
        float topY = -0.5f + ((i + 1.f) / m_param1);
        float bottomRadius = 0.5f - 0.5 * (i / float(m_param1));
        float topRadius = 0.5f - 0.5 * ((i + 1.f) / m_param1);

        for (int j = 0; j < m_param2; ++j) {
            float angle = j * angleStep;

            if (i == m_param1 - 1) {
                glm::vec3 p0(cos(angle) * bottomRadius, bottomY, sin(angle) * bottomRadius);
                glm::vec3 p1(cos(angle + angleStep) * bottomRadius, bottomY, sin(angle + angleStep) * bottomRadius);
                glm::vec3 p1Normal = glm::normalize(glm::vec4(2.f * p0[0], -(2.f * p0[1] - 1.f) / 4.f, 2.f * p0[2], 0.f));;
                glm::vec3 p2Normal = glm::normalize(glm::vec4(2.f * p1[0], -(2.f * p1[1] - 1.f) / 4.f, 2.f * p1[2], 0.f));;
                insertVec3(m_vertexData, tip);
                insertVec3(m_vertexData, glm::normalize(p1Normal + p2Normal));
                insertVec3(m_vertexData, p1);
                insertVec3(m_vertexData, p2Normal);
                insertVec3(m_vertexData, p0);
                insertVec3(m_vertexData, p1Normal);
            } else {
                glm::vec3 bottomLeft(cos(angle) * bottomRadius, bottomY, sin(angle) * bottomRadius);
                glm::vec3 bottomRight(cos(angle + angleStep) * bottomRadius, bottomY, sin(angle + angleStep) * bottomRadius);
                glm::vec3 topLeft(cos(angle) * topRadius, topY, sin(angle) * topRadius);
                glm::vec3 topRight(cos(angle + angleStep) * topRadius, topY, sin(angle + angleStep) * topRadius);

                glm::vec3 bottomLeftNormal = glm::normalize(glm::vec4(2.f * bottomLeft[0], -(2.f * bottomLeft[1] - 1.f) / 4.f, 2.f * bottomLeft[2], 0.f));
                glm::vec3 bottomRightNormal = glm::normalize(glm::vec4(2.f * bottomRight[0], -(2.f * bottomRight[1] - 1.f) / 4.f, 2.f * bottomRight[2], 0.f));;
                glm::vec3 topLeftNormal = glm::normalize(glm::vec4(2.f * topLeft[0], -(2.f * topLeft[1] - 1.f) / 4.f, 2.f * topLeft[2], 0.f));
                glm::vec3 topRightNormal = glm::normalize(glm::vec4(2.f * topRight[0], -(2.f * topRight[1] - 1.f) / 4.f, 2.f * topRight[2], 0.f));;

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

    for (int i = 0; i < m_param1; ++i) {
        for (int j = 0; j < m_param2; ++j) {
            float angle = j * angleStep;

            glm::vec3 bottomLeft(cos(angle) * 0.5f, -0.5f, sin(angle) * 0.5f);
            glm::vec3 bottomRight(cos(angle + angleStep) * 0.5f, -0.5f, sin(angle + angleStep) * 0.5f);
            glm::vec3 topLeft(cos(angle) * (0.5f / m_param1) * i, -0.5f, sin(angle) * (0.5f / m_param1) * i);
            glm::vec3 topRight(cos(angle + angleStep) * (0.5f / m_param1) * i, -0.5f, sin(angle + angleStep) * (0.5f / m_param1) * i);

            insertVec3(m_vertexData, bottomLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));

            insertVec3(m_vertexData, topRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, glm::normalize(glm::vec3(0.f, -1.f, 0.f)));
        }
    }
}


// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Cone::clearData() {
    m_vertexData = std::vector<float>();
}
