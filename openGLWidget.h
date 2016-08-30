#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <GL/glu.h>
#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QOpenGLWidget>

#include "trackBall.h"
#include "GLtexture.h"
#include "GLmodel.h"


class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);

  // GETTERS
    GLfloat getFramePictureRatio();
    qreal getCameraSettings(GLuint settingNumber);
    GLfloat getSensibility(), getSensibilityPlus();
    GLfloat getRotationSpeed();
    GLfloat getTagsRadius();
    GLfloat getTumorRadius();
    GLfloat getTumorDepth();
    QStringList getModelsList();

  // SETTERS
    void setCameraSettings(GLuint settingNumber, qreal newValue);
    void setFramePictureRatio(GLfloat new_frame_picture_Ratio);
    void setSensibility(GLfloat newValue), setSensibilityPlus(GLfloat newValue);
    void setRotationSpeed(GLfloat newValue);
    void scaleSliderState(bool newState);
    void setTagsRadius(GLfloat newValue);
    void setTumorRadius(GLfloat newValue);
    void setTumorDepth(GLfloat newValue);
    void setFrameByFrameMode(bool buttonChecked);

    void addModel(QString modelName);


private:
    TrackBall trackball;
    GLtexture backgroundTexture;
    GLmodel model;

    VideoCapture cap;
    bool frameByFrameMode;
    QString framesFolder;
    GLuint screenshotNumber;

    QVector<GLuint> checkedModels;
    QString selectedModel;
    GLint referenceModel;

    GLfloat frame_picture_Ratio;
    GLfloat scaleFactor;
    GLfloat sensibility, sensibilityPlus;
    GLfloat rotationSpeed;
    QPointF screenCoordinates;
    QVector3D surfaceCoordinates, distanceCoordinates1, distanceCoordinates2; // Coordinates displayed when model clicked
    QQuaternion tagsRotation;
    GLfloat distanceBetweenTags;
    qreal cameraParameters[8];
/*
 *  cameraParameters[0] = alphaX, focal (px)
 *  cameraParameters[1] = alphaY, focal (px)
 *  cameraParameters[2] = skewness
 *  cameraParameters[3] = u, image center abscissa (px)
 *  cameraParameters[4] = v, image center ordinate (px)
 *  cameraParameters[5] = near, distance to the nearer depth clipping plane (m)
 *  cameraParameters[6] = far, distance to the farther depth clipping plane (m)
 *  cameraParameters[7] = scale of the perspective view
 */

    bool scaleSliderPressed, distanceMode;
    GLuint picture, crosshair, tags; //DisplayLists
    GLfloat defaultTumorRadius, tumorDepth, tagsRadius; // Radius of the tumor, tags and depth of the tumor (m)
    QStringList modelsList;                 // List of loaded models

    void initializeGL();
    void paintGL();
    void resetCameraSettings();

    void updateModelsList();
    void releaseVideoCapture();
    void frameByFrameScreenshot(Mat frame);

    void resizeWidget();
    void createCrosshair();
    void createTags();

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);

    void multMatrix(const QMatrix4x4& m);
    QPointF pixelPosToViewPos(const QPointF &p);
    void screenToModelPixel();


signals:
    void pictureChanged(GLuint newWidth, GLuint newHeight);
    void distanceModeIsON(bool distanceModeON);
    void modelsChanged();
    void captureReleased();
    void takeScreenShot(QString path);
    void frameByFrameModeOFF(bool frameByFrameMode);


public slots:
  // BUTTONS
    void setTexturePath();
    bool setVideoPath(bool frameByframe);
    void updateVideo();

    void removeModels();
    void saveModels();
    void changeColor(QColor newColor);
    void setCheckedModels(QVector<GLuint> newCheckedModels);

    void addTexture();

    void setSelectedModel(QString newSelectedModel);
    void setReferenceModel(QString newReferenceModel);

    void createTumor();
    void centerModels();
    void setDistanceMode(bool buttonChecked);
    void rotateX();
    void rotateY();

  // SLIDER
    void setOpacity(int sliderValue);
};

#endif // OPENGLWIDGET_H
