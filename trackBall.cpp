#include "trackBall.h"


TrackBall::TrackBall(){}


void TrackBall::push(const QPointF& p)
{
    m_lastTime = QTime::currentTime();
    m_lastPos = p;
}

void TrackBall::move(const QPointF& p)
{
    QTime currentTime = QTime::currentTime();
    GLint msecs = m_lastTime.msecsTo(currentTime);
    if (msecs <= 60)
        return;

    QVector3D lastPos3D = QVector3D(m_lastPos.x(), m_lastPos.y(), 0.0f);
    GLfloat sqrZ = 1 - QVector3D::dotProduct(lastPos3D, lastPos3D);
    if (sqrZ > 0)
        lastPos3D.setZ(sqrt(sqrZ));
    else
        lastPos3D.normalize();

    QVector3D currentPos3D = QVector3D(p.x(), p.y(), 0.0f);
    sqrZ = 1 - QVector3D::dotProduct(currentPos3D, currentPos3D);
    if (sqrZ > 0)
        currentPos3D.setZ(sqrt(sqrZ));
    else
        currentPos3D.normalize();

    QVector3D m_axis = QVector3D::crossProduct(lastPos3D, currentPos3D);
    GLfloat angle = 180 / PI * asin(sqrt(QVector3D::dotProduct(m_axis, m_axis)));

    m_axis.normalize();
    QQuaternion transformation;
    m_axis = transformation.rotatedVector(m_axis);
    m_rotation = QQuaternion::fromAxisAndAngle(m_axis, angle) * m_rotation;
    m_lastPos = p;
    m_lastTime = currentTime;
}


QQuaternion TrackBall::rotation()     // Returns rotation
{
    return m_rotation;
}
void TrackBall::setRotation(QQuaternion r)  // Manually sets rotation to new value
{
    m_rotation = r;
}
