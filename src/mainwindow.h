#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include "realtime.h"
#include "utils/aspectratiowidget/aspectratiowidget.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();

private:
    void connectUIElements();
    void connectBuildingHeight();
    void connectBuildingIrregularity();
    void connectStreetDensityX();
    void connectStreetDensityZ();
    void connectFogDensity();
    void connectFogHeight();
    void connectSolidFog();
    void connectSaveImage();
    void connectText();

    Realtime *realtime;
    AspectRatioWidget *aspectRatioWidget;
    QPushButton *saveImage;
    QSlider *buildingHeightSlider;
    QSlider *buildingIrregularitySlider;
    QSlider *streetDensityXSlider;
    QSlider *streetDensityZSlider;
    QSlider *fogDensitySlider;
    QSlider *fogHeightSlider;
    QCheckBox *solidFogBox;
    QLineEdit *textBox;
    QComboBox *typefaceBox;

    QDoubleSpinBox *buildingHeightBox;
    QDoubleSpinBox *buildingIrregularityBox;
    QDoubleSpinBox *streetDensityXBox;
    QDoubleSpinBox *streetDensityZBox;
    QDoubleSpinBox *fogDensityBox;
    QDoubleSpinBox *fogHeightBox;

private slots:
    void onSaveImage();
    void onValChangeBuildingHeightSlider(int newValue);
    void onValChangeBuildingIrregularitySlider(int newValue);
    void onValChangeStreetDensityXSlider(int newValue);
    void onValChangeStreetDensityZSlider(int newValue);
    void onValChangeFogDensitySlider(int newValue);
    void onValChangeFogHeightSlider(int newValue);
    void onValChangeSolidFogBox();
    void onValChangeTextBox(QString newValue);
    void onValChangeTypefaceBox(QString newValue);

    void onValChangeBuildingHeightBox(double newValue);
    void onValChangeBuildingIrregularityBox(double newValue);
    void onValChangeStreetDensityXBox(double newValue);
    void onValChangeStreetDensityZBox(double newValue);
    void onValChangeFogDensityBox(double newValue);
    void onValChangeFogHeightBox(double newValue);
};
