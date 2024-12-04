#include "Sphere.h"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    glm::vec3 origin(0.f, 0.f, 0.f);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft - origin));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight - origin));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft - origin));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight - origin));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight - origin));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft - origin));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float r = 0.5f;
    float phi = glm::radians(180.f / m_param1);
    int width = 360.f / glm::degrees(nextTheta - currentTheta);
    for (int i = 0; i < m_param1; ++i) {
        glm::vec3 topLeft(r * glm::sin(i * phi) * glm::cos(currentTheta),
                          r * glm::cos(i * phi),
                          r * glm::sin(i * phi) * glm::sin(currentTheta));

        glm::vec3 topRight(r * glm::sin(i * phi) * glm::cos(nextTheta),
                           r * glm::cos(i * phi),
                           r * glm::sin(i * phi) * glm::sin(nextTheta));

        glm::vec3 bottomLeft(r * glm::sin((i + 1) * phi) * glm::cos(currentTheta),
                             r * glm::cos((i + 1) * phi),
                             r * glm::sin((i + 1) * phi) * glm::sin(currentTheta));

        glm::vec3 bottomRight(r * glm::sin((i + 1) * phi) * glm::cos(nextTheta),
                              r * glm::cos((i + 1) * phi),
                              r * glm::sin((i + 1) * phi) * glm::sin(nextTheta));

        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; ++i) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {
    makeSphere();
}

void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Sphere::clearData() {
    m_vertexData = std::vector<float>();
}
