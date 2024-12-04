#include <stdexcept>
#include "realtimescene.h"
#include "utils/sceneparser.h"

RealtimeScene::RealtimeScene(int width, int height, const RenderData &metaData)
    : sceneWidth(width), sceneHeight(height), globalData(metaData.globalData), camera(metaData.cameraData), metaData(metaData) {
}

const int& RealtimeScene::width() const {
    return sceneWidth;
}

const int& RealtimeScene::height() const {
    return sceneHeight;
}

const SceneGlobalData& RealtimeScene::getGlobalData() const {
    return globalData;
}

const Camera& RealtimeScene::getCamera() const {
    return camera;
}

const RenderData& RealtimeScene::getMetaData() const {
    return metaData;
}
