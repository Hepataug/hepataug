#include "trackBall.h"


TrackBall::TrackBall(){}


void TrackBall::push(const QPointF& p)  // Called when the user left-clicks on the OpenGLWidget
{
    lastTime = QTime::currentTime();  // Time is saved
    lastPos = p;                      // Position is saved
}

void TrackBall::move(const QPointF& p)  // Called when the user moves the mouse after a left-click on the OpenGLWidget
{
    if(lastTime.msecsTo(QTime::currentTime()) <= 60)  // Checking if the last move was done more than 60msecs ago
        return;

    QVector3D lastPos3D = QVector3D(lastPos.x(), lastPos.y(), 0);

    GLfloat sqrZ = 1 - pow(lastPos3D.length(), 2);  // Calculation of a z coordinate from x and y
    if(sqrZ > 0)
        lastPos3D.setZ(sqrt(sqrZ));
    else
        lastPos3D.normalize();

    QVector3D currentPos3D = QVector3D(p.x(), p.y(), 0);

    sqrZ = 1 - pow(currentPos3D.length(), 2);   // Calculation of a z' coordinate from x' and y'
    if(sqrZ > 0)
        currentPos3D.setZ(sqrt(sqrZ));
    else
        currentPos3D.normalize();

    QVector3D axis = QVector3D::crossProduct(lastPos3D, currentPos3D);   // Calculation of the rotation axis
    GLfloat angle = 180 / PI * asin(axis.length());                      // Calculation of the rotation angle

    rotationQ = QQuaternion::fromAxisAndAngle(axis, angle);   // Calculation of the rotation quaternion from axis and angle

    push(p);    // The current cursor position is saved as the last cursor position
}


QQuaternion TrackBall::rotation()     // Returns the rotation quaternion
{
    return rotationQ;
}
void TrackBall::setRotation(QQuaternion r)  // Manually sets rotation quaternion to new value
{
    rotationQ = r;
}
