#include "GLtexture.h"

GLtexture::GLtexture()
{
    glGenTextures(1, &texture[0]);
}


void GLtexture::setTexture(QString texturePath)
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    QImage qim_Texture = QImage(texturePath);
    qim_Texture = QGLWidget::convertToGLFormat(qim_Texture);

    width = qim_Texture.width();
    height = qim_Texture.height();

    glTexImage2D(GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void GLtexture::setTexture(Mat textureMat)
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

QImage GLtexture::matToQImage(Mat const& image)
{
    Mat temp;
    cvtColor(image, temp, CV_BGR2RGB);
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits();
    return dest;
}


void GLtexture::setResizeWidget(bool newValue)
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
