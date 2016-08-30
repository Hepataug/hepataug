#include "GLtexture.h"

GLtexture::GLtexture()  // Contains the background texture
{
    glGenTextures(1, &texture[0]);  // Texture initialization
    screenshotNumber = 1;   // Number of screenshots taken
}


void GLtexture::setTexture(QString texturePath) // Set texture from an image file
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    QImage qim_Texture = QImage(texturePath);
    qim_Texture = QGLWidget::convertToGLFormat(qim_Texture);

    width = qim_Texture.width();
    height = qim_Texture.height();

    glTexImage2D(GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void GLtexture::setTexture(Mat textureMat)  // Set texture from an OpenCV matrix
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    QImage qim_Texture = QGLWidget::convertToGLFormat(matToQImage(textureMat));

    if(resizeWidget)
    {
        width = qim_Texture.width();
        height = qim_Texture.height();
    }

    glTexImage2D(GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void GLtexture::saveTexture(Mat textureMat, QString path)   // Takes a screenshot of the texture into an image file
{
    QImage qim_Texture = matToQImage(textureMat);

    QString format = "png";
    QString fileName = path + QString("/background%1.%2").arg(screenshotNumber).arg(format);

    qim_Texture.save(fileName, qPrintable(format));

    screenshotNumber++;
}

QImage GLtexture::matToQImage(Mat const& image) // Convert an OpenCV matrix into a QImage
{
    Mat temp;
    cvtColor(image, temp, CV_BGR2RGB);
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits();
    return dest;
}


void GLtexture::setResizeWidget(bool newValue)  // Set if the OpenGLWidget will be resized with the texture size or not
{
    resizeWidget = newValue;
}

GLint GLtexture::getWidth()
{
    return width;
}
GLint GLtexture::getHeight()
{
    return height;
}

GLuint GLtexture::getTexture()
{
    return texture[0];
}
