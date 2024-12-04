#pragma once

#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "camera.h"

class RealtimeScene
{
private:
    int sceneWidth;
    int sceneHeight;
    SceneGlobalData globalData;
    Camera camera;
    RenderData metaData;
public:
    RealtimeScene(int width, int height, const RenderData &metaData);

    // The getter of the width of the scene
    const int& width() const;

    // The getter of the height of the scene
    const int& height() const;

    // The getter of the global data of the scene
    const SceneGlobalData& getGlobalData() const;

    // The getter of the shared pointer to the camera instance of the scene
    const Camera& getCamera() const;

    const RenderData& getMetaData() const;
};
