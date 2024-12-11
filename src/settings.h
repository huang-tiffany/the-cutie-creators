#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    std::string sceneFilePath;
    int shapeParameter1 = 5;
    int shapeParameter2 = 5;
    float nearPlane = 0.1;
    float farPlane = 100;
    std::string text;
    std::string typeface;
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
