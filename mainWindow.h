#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QToolBar>
#include <QToolTip>

#include "modelsListWidget.h"
#include "openGLWidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


private:
    QGridLayout *layoutPrincipal;
    OpenGLWidget *object;
    ModelsListWidget *list;


   // Left Toolbar
    QToolBar *leftToolBar;
    QAction *distance, *frameByFrame;


   // Right toolbar
    QToolBar *rightToolBar;
    QSlider *opacitySlider, *scaleSlider;


    GLuint screenshotNumber;    // Number of screenshots taken


   // Settings
    QLineEdit *framePictureRatioLineEdit, *rotationSpeedLineEdit, *tagsRadiusLineEdit, *tumorRadiusLineEdit, *tumorDepthLineEdit;
    QLineEdit *alphaX, *alphaY, *skewness, *centerX, *centerY, *near, *far;
    QLineEdit *sensibilityLineEdit, *sensibilityPlusLineEdit;


private slots :
    void resizeMainWindow(GLuint newWidth, GLuint newHeight);
    void distanceMode();
    void setVideoPath();
    void frameByFrameMode();
    void screenshot();
    void screenshot(QString path);
    void addModel();


   // Settings
    void settingsWindow();
    void sendSettings();


   // Sliders
    void updateToolTip(int sliderValue);
    void scaleSliderState();
    void focusOFF();

   // Models List
    void updateModelsList();
};

#endif // MAINWINDOW_H
