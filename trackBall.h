#ifndef TRACKBALL_H
#define TRACKBALL_H

#define PI 3.14159265

#include <GL/glu.h>
#include <QQuaternion>
#include <QTime>


class TrackBall
{
public:
    TrackBall();

    void push(const QPointF& p);
    void move(const QPointF& p);

    QQuaternion rotation();   // Returns rotation
    void setRotation(QQuaternion r);  // Manually sets rotation to new value


private:
    QQuaternion rotationQ;

    QPointF lastPos;
    QTime lastTime;
};

#endif // TRACKBALL_H
