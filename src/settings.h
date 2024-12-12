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
    bool solidFog = true;

    int buildingHeight = 1;
    int buildingIrregularity = 1;
    int streetDensityX = 1;
    int streetDensityZ = 1;

    float fogDensity = 0.75f;
    float fogHeight = 1.f;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
