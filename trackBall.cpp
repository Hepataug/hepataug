#include "trackBall.h"


TrackBall::TrackBall(){}


void TrackBall::push(const QPointF& p)
{
    m_lastTime = QTime::currentTime();
    m_lastPos = p;
}

void TrackBall::move(const QPointF& p)
{
    if(m_lastTime.msecsTo(QTime::currentTime()) <= 60)  // Checking if the last move was done more than 60msecs ago
        return;

    QVector3D lastPos3D = QVector3D(m_lastPos.x(), m_lastPos.y(), 0);
    GLfloat sqrZ = 1 - pow(lastPos3D.length(), 2);
    if(sqrZ > 0)
        lastPos3D.setZ(sqrt(sqrZ));
    else
        lastPos3D.normalize();

    QVector3D currentPos3D = QVector3D(p.x(), p.y(), 0);
    sqrZ = 1 - pow(currentPos3D.length(), 2);
    if(sqrZ > 0)
        currentPos3D.setZ(sqrt(sqrZ));
    else
        currentPos3D.normalize();

    QVector3D m_axis = QVector3D::crossProduct(lastPos3D, currentPos3D);
    GLfloat angle = 180 / PI * asin(m_axis.length());

    m_rotation = QQuaternion::fromAxisAndAngle(m_axis, angle);

    push(p);
}


QQuaternion TrackBall::rotation()     // Returns rotation
{
    return m_rotation;
}
void TrackBall::setRotation(QQuaternion r)  // Manually sets rotation to new value
{
    m_rotation = r;
}
