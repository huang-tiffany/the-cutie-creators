#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
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
    void connectParam1();
    void connectParam2();
    void connectNear();
    void connectFar();
    void connectBuildingHeight();
    void connectBuildingIrregularity();
    void connectStreetDensityX();
    void connectStreetDensityZ();
    void connectPerPixelFilter();
    void connectKernelBasedFilter();
    void connectUploadFile();
    void connectSaveImage();
<<<<<<< HEAD
    void connectExtraCredit();
    void connectText();
=======
>>>>>>> main

    Realtime *realtime;
    AspectRatioWidget *aspectRatioWidget;
    QCheckBox *filter1;
    QCheckBox *filter2;
    QPushButton *uploadFile;
    QPushButton *saveImage;
    QSlider *p1Slider;
    QSlider *p2Slider;
    QSpinBox *p1Box;
    QSpinBox *p2Box;
    QSlider *nearSlider;
    QSlider *farSlider;
    QSlider *buildingHeightSlider;
    QSlider *buildingIrregularitySlider;
    QSlider *streetDensityXSlider;
    QSlider *streetDensityZSlider;
    QDoubleSpinBox *nearBox;
    QDoubleSpinBox *farBox;
<<<<<<< HEAD
    QLineEdit *textBox;

    // Extra Credit:
    QCheckBox *ec1;
    QCheckBox *ec2;
    QCheckBox *ec3;
    QCheckBox *ec4;
=======
    QDoubleSpinBox *buildingHeightBox;
    QDoubleSpinBox *buildingIrregularityBox;
    QDoubleSpinBox *streetDensityXBox;
    QDoubleSpinBox *streetDensityZBox;
>>>>>>> main

private slots:
    void onPerPixelFilter();
    void onKernelBasedFilter();
    void onUploadFile();
    void onSaveImage();
    void onValChangeP1(int newValue);
    void onValChangeP2(int newValue);
    void onValChangeNearSlider(int newValue);
    void onValChangeFarSlider(int newValue);
    void onValChangeBuildingHeightSlider(int newValue);
    void onValChangeBuildingIrregularitySlider(int newValue);
    void onValChangeStreetDensityXSlider(int newValue);
    void onValChangeStreetDensityZSlider(int newValue);
    void onValChangeNearBox(double newValue);
    void onValChangeFarBox(double newValue);
<<<<<<< HEAD
    void onValChangeTextBox(QString newValue);

    // Extra Credit:
    void onExtraCredit1();
    void onExtraCredit2();
    void onExtraCredit3();
    void onExtraCredit4();
=======
    void onValChangeBuildingHeightBox(double newValue);
    void onValChangeBuildingIrregularityBox(double newValue);
    void onValChangeStreetDensityXBox(double newValue);
    void onValChangeStreetDensityZBox(double newValue);
>>>>>>> main
};
