#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <iostream>

void MainWindow::initialize() {
    realtime = new Realtime;
    aspectRatioWidget = new AspectRatioWidget(this);
    aspectRatioWidget->setAspectWidget(realtime, 3.f/4.f);
    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(aspectRatioWidget, 1);
    this->setLayout(hLayout);

    // Create labels in sidebox
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QLabel *text_label = new QLabel(); // Text label
    text_label->setText("Text");
    text_label->setFont(font);
    QLabel *text_string_label = new QLabel(); // Text label
    text_string_label->setText("Text String:");
    QLabel *typeface_label = new QLabel(); // Text label
    typeface_label->setText("Typeface:");
    QLabel *buildingHeight_label = new QLabel(); // Far plane label
    buildingHeight_label->setText("Building Height:");
    QLabel *buildingIrregularity_label = new QLabel(); // Far plane label
    buildingIrregularity_label->setText("Building Irregularity:");
    QLabel *streetDensityX_label = new QLabel(); // Far plane label
    streetDensityX_label->setText("Horizontal Street Density:");
    QLabel *streetDensityZ_label = new QLabel(); // Far plane label
    streetDensityZ_label->setText("Vertical Street Density:");
    
    saveImage = new QPushButton();
    saveImage->setText(QStringLiteral("Save image"));

    QGroupBox *textLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *ltext = new QHBoxLayout();
    QGroupBox *typefaceLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *ltypeface = new QHBoxLayout();

    QGroupBox *buildingHeightLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lbuildingHeight = new QHBoxLayout();
    QGroupBox *buildingIrregularityLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lbuildingIrregularity = new QHBoxLayout();
    QGroupBox *streetDensityXLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lstreetDensityX = new QHBoxLayout();
    QGroupBox *streetDensityZLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lstreetDensityZ = new QHBoxLayout();

    textBox = new QLineEdit();
    textBox->setText("Hello World");
    QStringList typefaces = { "Rubik Mono One", "Karla", "Jacquard 24", "Instrument Serif", "FK Grotesk Semi Mono" };
    typefaceBox = new QComboBox();
    typefaceBox->addItems(typefaces);

    buildingHeightSlider = new QSlider(Qt::Orientation::Horizontal); // Near plane slider
    buildingHeightSlider->setTickInterval(1);
    buildingHeightSlider->setMinimum(1);
    buildingHeightSlider->setMaximum(1000);
    buildingHeightSlider->setValue(500);

    buildingHeightBox = new QDoubleSpinBox();
    buildingHeightBox->setMinimum(1.f);
    buildingHeightBox->setMaximum(10.f);
    buildingHeightBox->setSingleStep(1.f);
    buildingHeightBox->setValue(5.f);

    buildingIrregularitySlider = new QSlider(Qt::Orientation::Horizontal); // Near plane slider
    buildingIrregularitySlider->setTickInterval(1);
    buildingIrregularitySlider->setMinimum(0);
    buildingIrregularitySlider->setMaximum(300);
    buildingIrregularitySlider->setValue(200);

    buildingIrregularityBox = new QDoubleSpinBox();
    buildingIrregularityBox->setMinimum(0.f);
    buildingIrregularityBox->setMaximum(3.f);
    buildingIrregularityBox->setSingleStep(1.f);
    buildingIrregularityBox->setValue(2.f);

    streetDensityXSlider = new QSlider(Qt::Orientation::Horizontal); // Near plane slider
    streetDensityXSlider->setTickInterval(1);
    streetDensityXSlider->setMinimum(0);
    streetDensityXSlider->setMaximum(5000);
    streetDensityXSlider->setValue(2000);

    streetDensityXBox = new QDoubleSpinBox();
    streetDensityXBox->setMinimum(0.f);
    streetDensityXBox->setMaximum(50.f);
    streetDensityXBox->setSingleStep(1.f);
    streetDensityXBox->setValue(20.f);

    streetDensityZSlider = new QSlider(Qt::Orientation::Horizontal); // Near plane slider
    streetDensityZSlider->setTickInterval(1);
    streetDensityZSlider->setMinimum(0);
    streetDensityZSlider->setMaximum(5000);
    streetDensityZSlider->setValue(2000);

    streetDensityZBox = new QDoubleSpinBox();
    streetDensityZBox->setMinimum(0.f);
    streetDensityZBox->setMaximum(50.f);
    streetDensityZBox->setSingleStep(1.f);
    streetDensityZBox->setValue(20.f);

    ltext->addWidget(textBox);
    ltypeface->addWidget(typefaceBox);
    textLayout->setLayout(ltext);
    typefaceLayout->setLayout(ltypeface);

    lbuildingHeight->addWidget(buildingHeightSlider);
    lbuildingHeight->addWidget(buildingHeightBox);
    buildingHeightLayout->setLayout(lbuildingHeight);

    lbuildingIrregularity->addWidget(buildingIrregularitySlider);
    lbuildingIrregularity->addWidget(buildingIrregularityBox);
    buildingIrregularityLayout->setLayout(lbuildingIrregularity);

    lstreetDensityX->addWidget(streetDensityXSlider);
    lstreetDensityX->addWidget(streetDensityXBox);
    streetDensityXLayout->setLayout(lstreetDensityX);

    lstreetDensityZ->addWidget(streetDensityZSlider);
    lstreetDensityZ->addWidget(streetDensityZBox);
    streetDensityZLayout->setLayout(lstreetDensityZ);

    vLayout->addWidget(saveImage);

    vLayout->addWidget(text_label);
    vLayout->addWidget(text_string_label);
    vLayout->addWidget(textLayout);
    vLayout->addWidget(typeface_label);
    vLayout->addWidget(typefaceLayout);

    vLayout->addWidget(buildingHeight_label);
    vLayout->addWidget(buildingHeightLayout);
    vLayout->addWidget(buildingIrregularity_label);
    vLayout->addWidget(buildingIrregularityLayout);
    vLayout->addWidget(streetDensityX_label);
    vLayout->addWidget(streetDensityXLayout);
    vLayout->addWidget(streetDensityZ_label);
    vLayout->addWidget(streetDensityZLayout);

    connectUIElements();

    onValChangeTextBox("Hello World");
    onValChangeTypefaceBox("Q");
    onValChangeBuildingHeightBox(5.f);
    onValChangeBuildingIrregularityBox(2.f);
    onValChangeStreetDensityXBox(20.f);
    onValChangeStreetDensityZBox(20.f);
}

void MainWindow::finish() {
    realtime->finish();
    delete(realtime);
}

void MainWindow::connectUIElements() {
    connectSaveImage();
    connectText();
    connectBuildingHeight();
    connectBuildingIrregularity();
    connectStreetDensityX();
    connectStreetDensityZ();
}

void MainWindow::connectSaveImage() {
    connect(saveImage, &QPushButton::clicked, this, &MainWindow::onSaveImage);
}

void MainWindow::connectText() {
    connect(textBox, &QLineEdit::textChanged, this, &MainWindow::onValChangeTextBox);
    connect(typefaceBox, &QComboBox::currentTextChanged, this, &MainWindow::onValChangeTypefaceBox);
}

void MainWindow::connectBuildingHeight() {
    connect(buildingHeightSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeBuildingHeightSlider);
    connect(buildingHeightBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeBuildingHeightBox);
}

void MainWindow::connectBuildingIrregularity() {
    connect(buildingIrregularitySlider, &QSlider::valueChanged, this, &MainWindow::onValChangeBuildingIrregularitySlider);
    connect(buildingIrregularityBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeBuildingIrregularityBox);
}

void MainWindow::connectStreetDensityX() {
    connect(streetDensityXSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeStreetDensityXSlider);
    connect(streetDensityXBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeStreetDensityXBox);
}

void MainWindow::connectStreetDensityZ() {
    connect(streetDensityZSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeStreetDensityZSlider);
    connect(streetDensityZBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeStreetDensityZBox);
}

void MainWindow::onSaveImage() {
    if (settings.sceneFilePath.empty()) {
        std::cout << "No scene file loaded." << std::endl;
        return;
    }
    std::string sceneName = settings.sceneFilePath.substr(0, settings.sceneFilePath.find_last_of("."));
    sceneName = sceneName.substr(sceneName.find_last_of("/")+1);
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    QDir::currentPath()
                                                        .append(QDir::separator())
                                                        .append("student_outputs")
                                                        .append(QDir::separator())
                                                        .append("action")
                                                        .append(QDir::separator())
                                                        .append("required")
                                                        .append(QDir::separator())
                                                        .append(sceneName), tr("Image Files (*.png)"));
    std::cout << "Saving image to: \"" << filePath.toStdString() << "\"." << std::endl;
    realtime->saveViewportImage(filePath.toStdString());
}

void MainWindow::onValChangeTextBox(QString newValue) {
    textBox->setText(newValue);
    settings.text = textBox->text().toStdString();
    realtime->settingsChanged();
}

void MainWindow::onValChangeTypefaceBox(QString newValue) {
    typefaceBox->setCurrentText(newValue);
    settings.typeface = typefaceBox->currentText().toStdString();
    realtime->settingsChanged();
}

void MainWindow::onValChangeBuildingHeightSlider(int newValue) {
    //farSlider->setValue(newValue);
    buildingHeightBox->setValue(newValue/100.f);
    settings.buildingHeight = buildingHeightBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeBuildingHeightBox(double newValue) {
    buildingHeightSlider->setValue(int(newValue*100.f));
    //farBox->setValue(newValue);
    settings.buildingHeight = buildingHeightBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeBuildingIrregularitySlider(int newValue) {
    //farSlider->setValue(newValue);
    buildingIrregularityBox->setValue(newValue/100.f);
    settings.buildingIrregularity = buildingIrregularityBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeBuildingIrregularityBox(double newValue) {
    buildingIrregularitySlider->setValue(int(newValue*100.f));
    //farBox->setValue(newValue);
    settings.buildingIrregularity = buildingIrregularityBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeStreetDensityXSlider(int newValue) {
    //farSlider->setValue(newValue);
    streetDensityXBox->setValue(newValue/100.f);
    settings.streetDensityX = streetDensityXBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeStreetDensityXBox(double newValue) {
    streetDensityXSlider->setValue(int(newValue*100.f));
    //farBox->setValue(newValue);
    settings.streetDensityX = streetDensityXBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeStreetDensityZSlider(int newValue) {
    //farSlider->setValue(newValue);
    streetDensityZBox->setValue(newValue/100.f);
    settings.streetDensityZ = streetDensityZBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeStreetDensityZBox(double newValue) {
    streetDensityZSlider->setValue(int(newValue*100.f));
    //farBox->setValue(newValue);
    settings.streetDensityZ = streetDensityZBox->value();
    realtime->settingsChanged();
}
