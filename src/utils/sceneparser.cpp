#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

void fillData(SceneNode& curr, glm::mat4& ctm, std::vector<RenderShapeData>& primitives, std::vector<SceneLightData>& lights) {
    glm::mat4 currCtm = ctm;
    for (SceneTransformation* transform : curr.transformations) {
        switch (transform->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            currCtm *= glm::translate(glm::mat4(1.0f), transform->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            currCtm *= glm::scale(glm::mat4(1.0f), transform->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            currCtm *= glm::rotate(glm::mat4(1.0f), transform->angle, transform->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            currCtm *= transform->matrix;
            break;
        }
    }
    for (ScenePrimitive* primitive : curr.primitives) {
        RenderShapeData shapeData;
        shapeData.primitive = *primitive;
        shapeData.ctm = currCtm;
        primitives.push_back(shapeData);
    }
    for (SceneLight* light : curr.lights) {
        glm::vec4 pos = currCtm * glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 dir = currCtm * light->dir;
        SceneLightData lightData;
        lightData.id = light->id;
        lightData.type = light->type;
        lightData.color = light->color;
        lightData.function = light->function;
        lightData.pos = pos;
        lightData.dir = dir;
        lightData.penumbra = light->penumbra;
        lightData.angle = light->angle;
        lightData.width = light->width;
        lightData.height = light->height;
        lights.push_back(lightData);
    }
    for (SceneNode* child : curr.children) {
        fillData(*child, currCtm, primitives, lights);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }
    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    SceneNode* root = fileReader.getRootNode();
    renderData.shapes.clear();
    glm::mat4 ctm = glm::mat4(1.f);
    fillData(*root, ctm, renderData.shapes, renderData.lights);
    return true;
}
