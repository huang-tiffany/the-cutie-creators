#include "fog.h"

#include <cmath>
#include "glm/glm.hpp"
#include "settings.h"

Fog::Fog()
{
    m_wireshade = false;

    m_resolution = 100;

    m_lookupSize = 1024;
    m_randVecLookup.reserve(m_lookupSize);

    std::srand(1230);

    for (int i = 0; i < m_lookupSize; i++) {
        m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,
                                            std::rand() * 2.0 / RAND_MAX - 1.0));
    }
}

Fog::~Fog()
{
    m_randVecLookup.clear();
}

void addPointToVector(glm::vec4 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
    vector.push_back(point.w);
}

void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

std::vector<float> Fog::generateFog() {
    std::vector<float> verts;
    verts.reserve(m_resolution * m_resolution * 6);

    for(int x = 0; x < m_resolution; x++) {
        for(int y = 0; y < m_resolution; y++) {
            int x1 = x;
            int y1 = y;

            int x2 = x + 1;
            int y2 = y + 1;

            glm::vec3 p1 = getPosition(x1,y1);
            glm::vec3 p2 = getPosition(x2,y1);
            glm::vec3 p3 = getPosition(x2,y2);
            glm::vec3 p4 = getPosition(x1,y2);

            glm::vec3 n1 = getNormal(x1,y1);
            glm::vec3 n2 = getNormal(x2,y1);
            glm::vec3 n3 = getNormal(x2,y2);
            glm::vec3 n4 = getNormal(x1,y2);

            addPointToVector(glm::vec4(p1, 1.f), verts);
            addPointToVector(n1, verts);

            addPointToVector(glm::vec4(p2, 1.f), verts);
            addPointToVector(n2, verts);

            addPointToVector(glm::vec4(p3, 1.f), verts);
            addPointToVector(n3, verts);

            addPointToVector(glm::vec4(p1, 1.f), verts);
            addPointToVector(n1, verts);

            addPointToVector(glm::vec4(p3, 1.f), verts);
            addPointToVector(n3, verts);

            addPointToVector(glm::vec4(p4, 1.f), verts);
            addPointToVector(n4, verts);
        }
    }
    return verts;
}

glm::vec2 Fog::sampleRandomVector(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

glm::vec3 Fog::getPosition(int row, int col) {
    float pixelOffset = 50.f;
    float stretch = 60.f;

    float x = stretch * row / m_resolution;
    float z = stretch * col / m_resolution;

    float originalX = x;
    float originalZ = z;

    x -= pixelOffset;
    z -= pixelOffset;

    float y = getHeight((originalX - pixelOffset) / (0.9f), (originalZ - pixelOffset) / (0.9f));
    y *= 0.2f * settings.fogHeight;
    y += 0.03f;

    return glm::vec3(x, y, z);
}

float interpolate(float A, float B, float alpha) {
    return A + (3 * alpha * alpha - 2 * alpha * alpha * alpha) * (B - A);
}

float Fog::getHeight(float x, float y) {
    float z = computePerlin(x * 2, y * 2) / 2;
    z += computePerlin(x * 4, y * 4) / 4;
    z += computePerlin(x * 4, y * 4) / 4;
    z += computePerlin(x * 8, y * 8) / 8;
    z += computePerlin(x * 8, y * 8) / 8;
    z += computePerlin(x * 16, y * 16) / 16;
    z += computePerlin(x * 32, y * 32) / 32;

    return z;
}

glm::vec3 Fog::getNormal(int row, int col) {
    glm::vec3 normal(0,0,0);
    std::vector<std::vector<int>> neighborOffsets = {
        {-1, -1},
        { 0, -1},
        { 1, -1},
        { 1,  0},
        { 1,  1},
        { 0,  1},
        {-1,  1},
        {-1,  0}
    };

    glm::vec3 V = getPosition(row,col);
    for (int i = 0; i < neighborOffsets.size(); ++i) {
        int n2Index = (i + 1) % 8;
        glm::vec3 n1 = getPosition(row + neighborOffsets[i][0], col + neighborOffsets[i][1]);
        glm::vec3 n2 = getPosition(row + neighborOffsets[n2Index][0], col + neighborOffsets[n2Index][1]);
        normal = normal + glm::cross(n1 - V, n2 - V);
    }
    return glm::normalize(normal);
}

float Fog::computePerlin(float x, float y) {
    int leftX = floor(x);
    int rightX = leftX + 1;
    int topY = floor(y);
    int bottomY = topY + 1;

    glm::vec2 offsetTopLeft(x - leftX, y - topY);
    glm::vec2 offsetTopRight(x - rightX, y - topY);
    glm::vec2 offsetBottomLeft(x - leftX, y - bottomY);
    glm::vec2 offsetBottomRight(x - rightX, y - bottomY);

    float A = glm::dot(sampleRandomVector(leftX, topY), offsetTopLeft);
    float B = glm::dot(sampleRandomVector(rightX, topY), offsetTopRight);
    float C = glm::dot(sampleRandomVector(rightX, bottomY), offsetBottomRight);
    float D = glm::dot(sampleRandomVector(leftX, bottomY), offsetBottomLeft);

    float dx = x - float(leftX);
    float dy = y - float(topY);

    return interpolate(interpolate(A, B, dx), interpolate(D, C, dx), dy);
}
