#include "cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    insertVec3(m_vertexData, topLeft);
    m_vertexData.push_back(1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(topLeft - bottomLeft, topLeft - bottomRight)));
    insertVec3(m_vertexData, bottomLeft);
    m_vertexData.push_back(1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(bottomLeft - bottomRight, bottomLeft - topLeft)));
    insertVec3(m_vertexData, bottomRight);
    m_vertexData.push_back(1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(bottomRight - topLeft, bottomRight - bottomLeft)));

    insertVec3(m_vertexData, topRight);
    m_vertexData.push_back(1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(topRight - topLeft, topRight - bottomRight)));
    insertVec3(m_vertexData, topLeft);
    m_vertexData.push_back(1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(topLeft - bottomRight, topLeft - topRight)));
    insertVec3(m_vertexData, bottomRight);
    m_vertexData.push_back(1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(bottomRight - topRight, bottomRight - topLeft)));
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 tileWidth = (topRight - topLeft) / static_cast<float>(m_param1);
    glm::vec3 tileHeight = (bottomLeft - topLeft) / static_cast<float>(m_param1);

    for (int i = 0; i < m_param1; ++i) {
        for (int j = 0; j < m_param1; ++j) {
            glm::vec3 currentTopLeft = topLeft + glm::vec3(i * tileWidth.x, j * tileHeight.y, i * tileWidth.z + j * tileHeight.z);
            glm::vec3 currentTopRight = currentTopLeft + tileWidth;
            glm::vec3 currentBottomLeft = currentTopLeft + tileHeight;
            glm::vec3 currentBottomRight = currentBottomLeft + tileWidth;
            makeTile(currentTopLeft, currentTopRight, currentBottomLeft, currentBottomRight);
        }
    }
}

void Cube::setVertexData() {
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    makeFace(glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3( -0.5f, -0.5f, -0.5f));

    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f));

    makeFace(glm::vec3(0.5f,  0.5f, 0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f));

    makeFace(glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(0.5f, 0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f, -0.5f));

    makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f));
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Cube::clearData() {
    m_vertexData = std::vector<float>();
}
