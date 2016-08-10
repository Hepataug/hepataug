#ifndef GLMODEL_H
#define GLMODEL_H

#include <QFileDialog>
#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector2D>


struct Face{
    QVector<QVector4D> v, vn;
    QVector<QVector3D> t;

    Face(){v.resize(4);
           vn.resize(4);
           t.resize(4);}
};

struct Model{

public :
    GLuint texture[1], model;
    bool squareFile, vnFile, vtFile;
    QString modelName, fileName, mtllib, textureName;
    QVector3D position, origin;
    QVector4D color;
    QQuaternion rotation;

    Model(){squareFile = vnFile = vtFile = false;
            texture[0] = 0;
            model = 0;
            color = QVector4D(1.f, 1.f, 1.f, 1.f);}
};


class GLmodel{

public:
    GLmodel();

    void loadModel(QString fileName, GLuint modelNumber);
    void saveModel(QStringList newModelsNames, QVector<GLuint> modelsNumber, bool assemble);
    void removeModel(GLuint modelNumber);

    Model getModelSettings(GLuint modelNumber); // Get model settings
    GLuint getModelSize();

    void setModelSettings(Model newModel, GLuint modelNumber);
    void loadTexture(QString textureName, GLuint modelNumber);

private:
    QVector<Face> Faces;    //f
    QVector<Model> Models;  // Models list

    void loadMTL(QString MTLPath, GLuint modelNumber);


  // OPENGL TASKS
    void calculateNormal(GLuint i, GLuint modelNumber);
    void drawFace(GLuint i, GLuint j, GLuint modelNumber);
};

#endif // GLMODEL_H
