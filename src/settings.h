#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    std::string sceneFilePath;
    int shapeParameter1 = 1;
    int shapeParameter2 = 1;
    float nearPlane = 1;
    float farPlane = 1;
    std::string text;
    bool perPixelFilter = false;
    bool kernelBasedFilter = false;

    int buildingHeight = 1;
    int buildingIrregularity = 1;
    int streetDensityX = 1;
    int streetDensityZ = 1;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
