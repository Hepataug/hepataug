#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <QGLWidget>
#include <opencv2/opencv.hpp>

using namespace cv;


class GLtexture
{
public:
    GLtexture();

    void setTexture(QString texturePath);
    void setTexture(Mat textureMat);
    void saveTexture(Mat textureMat, QString path);
    void setResizeWidget(bool newValue);

    GLint getWidth();
    GLint getHeight();

    GLuint getTexture();


private:
    QImage matToQImage(Mat const& image);

    GLuint texture[1];
    GLuint screenshotNumber;    // Number of screenshots taken
    bool resizeWidget;  // Set if the OpenGLWidget will be resized with the texture size or not

    GLint width, height;    // Texture size
};

#endif // GLTEXTURE_H
