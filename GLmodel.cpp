#include "GLmodel.h"


GLmodel::GLmodel(){}


void GLmodel::loadModel(QString fileName, GLuint firstModelNumber)
{
    /* ============================ READING FILE ============================ */

    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream fileText(&file);
    QString buffer;
    bool loop = true;
    GLuint numberObject = 0;

    for(GLuint modelNumber = firstModelNumber; loop; modelNumber++)
    {
        QVector<QVector3D> Vertices, VNormals;  // v, vn
        QVector<QVector2D> VTexture;            // vt

        GLfloat xMin, yMin, zMin, xMax, yMax, zMax;   // Extreme coordinates to calculate origin
        xMin = yMin = zMin = xMax = yMax = zMax = 0;

        QVector3D temp3D;   // Buffer vectors
        QVector2D temp2D;
        loop = false;

        Model currentModel;     // New Model
        Models.push_back(currentModel); // Adding new model to the list
        Models[modelNumber].fileName = fileName;
        Models[modelNumber].modelName = buffer;

        while(!fileText.atEnd())
        {
            QString fileLine = fileText.readLine();
            fileLine = fileLine.simplified();

          // O
            if(fileLine.startsWith("o "))
            {
                numberObject++;
                QStringList lineList = fileLine.split(" ");

                if(numberObject>1)
                {
                    buffer = lineList[1];
                    loop = true;
                    break;
                }

                Models[modelNumber].modelName = lineList[1];
                buffer.clear();
            }

          // VN
            if(fileLine.startsWith("vn "))
            {
                Models[modelNumber].vnFile = true;

                while(fileLine.contains(","))       // If necessary, replaces ',' with '.'
                    fileLine = fileLine.replace(fileLine.indexOf(",", 1),1,".");

                QStringList lineList = fileLine.split(" ");

                temp3D = QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
                VNormals.push_back(temp3D);
            }

          // VT
            else if(fileLine.startsWith("vt "))
            {
                Models[modelNumber].vtFile = true;

                while(fileLine.contains(","))       // If necessary, replaces ',' with '.'
                    fileLine = fileLine.replace(fileLine.indexOf(",", 1),1,".");

                QStringList lineList = fileLine.split(" ");

                temp2D = QVector2D(lineList[1].toFloat(), lineList[2].toFloat());
                VTexture.push_back(temp2D);
            }

          // V
            else if(fileLine.startsWith("v "))
            {
                while(fileLine.contains(","))       // If necessary, replaces ',' with '.'
                    fileLine = fileLine.replace(fileLine.indexOf(",", 1),1,".");

                QStringList lineList = fileLine.split(" ");

                temp3D = QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
                Vertices.push_back(temp3D);

                if (xMin==0 || temp3D.x() < xMin)   // Records the min and max coordinates values...
                    xMin = temp3D.x();              //...to calculate the model center "origin"
                if (yMin==0 || temp3D.y() < yMin)
                    yMin = temp3D.y();
                if (zMin==0 || temp3D.z() < zMin)
                    zMin = temp3D.z();
                if (xMax==0 || temp3D.x() > xMax)
                    xMax = temp3D.x();
                if (yMax==0 || temp3D.y() > yMax)
                    yMax = temp3D.y();
                if (zMax==0 || temp3D.z() > zMax)
                    zMax = temp3D.z();
            }

          // F
            else if(fileLine.startsWith("f "))
            {
                Face F;
                QStringList lineList = fileLine.split(" ");

                for(int i = 1; i <= 3; i++)
                {
                    QStringList arg = lineList[i].split("/");

                    F.v[i-1] = Vertices[arg[0].toInt()-1];
                    if(Models.at(modelNumber).vtFile)
                        F.t[i-1] = VTexture[arg[1].toInt()-1];
                    if(Models.at(modelNumber).vnFile)
                        F.vn[i-1] = VNormals[arg[2].toInt()-1];
                }
                if(lineList.size()==5)
                {
                    Models[modelNumber].squareFile = true;
                    QStringList arg = lineList[4].split("/");

                    F.v[3] = Vertices[arg[0].toInt()-1];
                    if(Models.at(modelNumber).vtFile)
                        F.t[3] = VTexture[arg[1].toInt()-1];
                    if(Models.at(modelNumber).vnFile)
                        F.vn[3] = VNormals[arg[2].toInt()-1];
                }
                Faces.push_back(F);
            }

          // MTLLIB
            else if(fileLine.startsWith("mtllib "))
            {
                QStringList lineList = fileLine.split(" ");
                Models[modelNumber].mtllib = lineList[1];
            }
        }

        if(Models[modelNumber].modelName.isEmpty())
            Models[modelNumber].modelName = fileName;


        QFileInfo fi(fileName);
        QString BaseName = fi.fileName();
        QString MTLPath(fileName);
        MTLPath.remove(MTLPath.size() - BaseName.size(), BaseName.size());

        if(!Models.at(modelNumber).mtllib.isEmpty())
            loadMTL(MTLPath, modelNumber);
        else
            Models[modelNumber].color = QVector4D(1.f, 0.f, 0.f, 1.f);


      // Calculates the model center coordinates
        Models[modelNumber].origin = QVector3D(xMin+((xMax-xMin)/2), yMin+((yMax-yMin)/2), zMin+((zMax-zMin)/2));
        Models[modelNumber].position = Models.at(modelNumber).origin;


    /* ============================ DRAW MODEL INTO DISPLAYLIST ============================ */

        Models[modelNumber].model = glGenLists(1);
        glNewList(Models[modelNumber].model, GL_COMPILE);
            glBegin(GL_TRIANGLES);
                for(GLuint i = 0; i < (GLuint)Faces.size(); i++)
                    if(Faces[i].v[3].isNull())
                    {
                        calculateNormal(i, modelNumber);
                        for(GLuint j = 0; j < 3; j++)
                            drawFace(i, j, modelNumber);
                    }
            glEnd();

            if(Models.at(modelNumber).squareFile)
            {
                glBegin(GL_QUADS);
                    for(GLuint i = 0; i < (GLuint)Faces.size(); i++)
                        if(!Faces[i].v[3].isNull())
                        {
                            calculateNormal(i, modelNumber);
                            for(GLuint j = 0; j < 4; j++)
                                drawFace(i, j, modelNumber);
                        }
                glEnd();
            }
        glEndList();

        Faces.clear();
    }

    file.close();
}


void GLmodel::saveModel(QStringList newModelsNames, QVector<GLuint> modelsNumber, bool assemble)
{
    if(assemble)
    {
        QChar newModelName_ch[newModelsNames.at(0).size()];     // Checking if .obj extension was seized during file creation
        QString extension;
        for(GLuint i = 0; i < (GLuint)newModelsNames.at(0).size(); i++)
        {
            newModelName_ch[i] = newModelsNames.at(0).at(i);
            if(newModelName_ch[i] == '.')
                for(; i < (GLuint)newModelsNames.at(0).size(); i++)
                    extension += newModelsNames.at(0).at(i);
        }
        if(extension=="")
            QTextStream(&newModelsNames[0]) << ".obj";


        /* ============================ FILE WRITING ============================ */

        QFile newFile(newModelsNames.at(0));
        newFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QFileInfo fileInfo(newModelsNames.at(0));


        /* ============================ HEADER ============================ */

        QTextStream fluxOut(&newFile);
        fluxOut.setCodec("UTF-8");
        fluxOut << "####" <<endl
        << "#" <<endl
        << "# OBJ File Generated by HEPATAUG" <<endl
        << "#" <<endl
        << "####" <<endl
        << "#" <<endl
        << "# Object " << fileInfo.fileName() <<endl
        << "#" <<endl
        << "####" <<endl;


        for(GLuint n = 0; n < (GLuint)modelsNumber.size(); n++)
        {
            /* ============================ FILE READING ============================ */

            QString modelName = Models.at(modelsNumber.at(n)).fileName;
            QFile file(modelName);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream fileText(&file);

            fluxOut <<endl<< "o " << Models.at(modelsNumber.at(n)).modelName <<endl<<endl;


            QString path = Models.at(modelsNumber.at(n)).fileName;
            QStringList lineList = path.split('/');
            path = path.remove(path.indexOf(lineList.last()), lineList.at(lineList.size()-1).length());
            QString newPath = newFile.fileName();
            QStringList lineList2 = newPath.split('/');
            newPath = newPath.remove(newPath.indexOf(lineList2.last()), lineList2.at(lineList2.size()-1).length());


            if(!Models.at(modelsNumber.at(n)).mtllib.isEmpty())
                fluxOut << "mtllib " << Models.at(modelsNumber.at(n)).mtllib <<endl<<endl;

            else if(!Models.at(modelsNumber.at(n)).textureName.isEmpty())
            {
                QStringList lineList = Models.at(modelsNumber.at(n)).textureName.split('/');
                QFile texture(Models.at(modelsNumber.at(n)).textureName);
                texture.copy(newPath + lineList.last());

                QStringList nameList = Models.at(modelsNumber.at(n)).modelName.split('/');
                QString name = nameList.last().split('.').at(0) + QString(".mtl");

                QFile newMtllibFile(name);

                if (!newMtllibFile.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

                QTextStream flux(&newMtllibFile);
                flux.setCodec("UTF-8");
                flux << "map_Kd " << lineList.last();

                newMtllibFile.close();

                fluxOut << "mtllib " << name <<endl<<endl;
            }

            QString fileLine;

            while(!fileLine.contains(Models.at(modelsNumber.at(n)).modelName) && !fileText.atEnd())
                fileLine = fileText.readLine();

            if(fileText.atEnd())
                fileText.seek(0);

            while(!fileText.atEnd())
            {
                fileLine = fileText.readLine();
                fileLine = fileLine.simplified();

            /* ============================ BODY - VERTICES ============================ */

                QMatrix4x4 r;
                r.rotate(Models.at(modelsNumber.at(n)).rotation);

                if(fileLine.startsWith("v "))
                {
                    QStringList lineList = fileLine.split(" ");
                    QVector3D coords(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());

                    if(Models.at(modelsNumber.at(n)).tumorRadius > 0)
                        coords = r * (coords*Models.at(modelsNumber.at(n)).tumorRadius/0.33 - Models.at(modelsNumber.at(n)).origin) + Models.at(modelsNumber.at(n)).position;
                    else
                        coords = r * (coords - Models.at(modelsNumber.at(n)).origin) + Models.at(modelsNumber.at(n)).position;

                    fluxOut << "v " << QString::number(coords.x(), 'f', 6)
                            << " " << QString::number(coords.y(), 'f', 6)
                            << " " << QString::number(coords.z(), 'f', 6) <<endl;
                }


            /* ============================ BODY - NORMAL ============================ */

                if(Models.at(modelsNumber.at(n)).vnFile)
                    if(fileLine.startsWith("vn "))
                    {
                        QStringList lineList = fileLine.split(" ");
                        QVector3D coords(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());

                        coords = r * (coords);

                        fluxOut << "vn " << QString::number(coords.x(), 'f', 6)
                                << " " << QString::number(coords.y(), 'f', 6)
                                << " " << QString::number(coords.z(), 'f', 6) <<endl;
                    }


            /* ============================ BODY - TEXTURE ============================ */

                if(Models.at(modelsNumber.at(n)).vtFile)
                    if(fileLine.startsWith("vt "))
                    {
                        QStringList lineList = fileLine.split(" ");
                        fluxOut << "vt " << lineList[1] << " " << lineList[2] <<endl;
                    }


            /* ============================ BODY - FACES ============================ */

                if(fileLine.startsWith("f "))
                {
                    QStringList lineList = fileLine.split(" ");

                    fluxOut << "f " << lineList[1] <<" "<< lineList[2] <<" "<< lineList[3];

                    if(lineList.size() > 4)
                        for(GLuint i = 4; i < (GLuint)lineList.size(); i++)
                            fluxOut<<" "<< lineList[i];

                    fluxOut<<endl;
                }

                if(fileLine.startsWith("o "))
                    break;
            }
            file.close();

            if(!Models.at(modelsNumber.at(n)).mtllib.isEmpty())
            {
                QString path = Models.at(modelsNumber.at(n)).fileName;
                QStringList lineList = path.split('/');
                path = path.remove(path.indexOf(lineList.last()), lineList.at(lineList.size()-1).length());

                QFile mtllibFile(path + Models.at(modelsNumber.at(n)).mtllib);

                QString newPath = newFile.fileName();
                QStringList lineList2 = newPath.split('/');
                newPath = newPath.remove(newPath.indexOf(lineList2.last()), lineList2.at(lineList2.size()-1).length());

                mtllibFile.copy(newPath + Models.at(modelsNumber.at(n)).mtllib);


                if(mtllibFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream fileText(&mtllibFile);
                    bool textureFound = false;

                    while(!fileText.atEnd() && !textureFound)
                    {
                        QString fileLine = fileText.readLine();
                        fileLine.simplified();

                        if(fileLine.contains("map_Kd"))
                        {
                            textureFound = true;
                            QStringList lineList = fileLine.split(" ");
                            QString textureName = path + lineList[fileLine.indexOf("map_Kd")+1];
                            while(textureName.contains("\\"))       // If necessary, replaces '\' with '/'
                                textureName.replace(textureName.indexOf("\\", 1),1,"/");
                            while(textureName.contains(".."))       // If necessary, removes ".."
                                textureName.remove(textureName.indexOf("..", 1),2);
                            while(textureName.contains("\\"))       // If necessary, replaces "//" with '/'
                                textureName.replace(textureName.indexOf("//", 1),2,"/");

                            QFile texture(textureName);
                            texture.copy(newPath + lineList[fileLine.indexOf("map_Kd")+1]);
                        }
                    }
                    mtllibFile.close();
                }
            }

            /* ============================ FOOTER ============================ */
            fluxOut << "\n# End of File ";
        }
        newFile.close();
    }

    else
        for(GLuint n = 0; n < (GLuint)modelsNumber.size(); n++)
        {
            QChar newModelName_ch[newModelsNames.at(n).size()];     // Checking if .obj extension was seized during file creation
            QString extension;
            for(GLuint i = 0; i < (GLuint)newModelsNames.at(n).size(); i++)
            {
                newModelName_ch[i] = newModelsNames.at(n).at(i);
                if(newModelName_ch[i]=='.')
                    for(;i < (GLuint)newModelsNames.at(n).size(); i++)
                        extension += newModelsNames.at(n).at(i);
            }
            if(extension=="")
                QTextStream(&newModelsNames[n])<<".obj";

            /* ============================ FILE WRITING ============================ */

            QFile newFile(newModelsNames.at(n));
            newFile.open(QIODevice::WriteOnly | QIODevice::Text);
            QFileInfo fileInfo(newModelsNames.at(n));


            /* ============================ FILE READING ============================ */

            QString modelName = Models.at(modelsNumber.at(n)).modelName;
            QFile file(modelName);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream fileText(&file);


            /* ============================ HEADER ============================ */

            QTextStream fluxOut(&newFile);
            fluxOut.setCodec("UTF-8");
            fluxOut << "####" <<endl
            << "#" <<endl
            << "# OBJ File Generated by HEPATAUG" <<endl
            << "#" <<endl
            << "####" <<endl
            << "#" <<endl
            << "# Object " << fileInfo.fileName() <<endl
            << "#" <<endl
            << "####" <<endl<<endl;


            QString path = Models.at(modelsNumber.at(n)).fileName;
            QStringList lineList = path.split('/');
            path = path.remove(path.indexOf(lineList.last()), lineList.at(lineList.size()-1).length());
            QString newPath = newFile.fileName();
            QStringList lineList2 = newPath.split('/');
            newPath = newPath.remove(newPath.indexOf(lineList2.last()), lineList2.at(lineList2.size()-1).length());


            if(!Models.at(modelsNumber.at(n)).mtllib.isEmpty())
                fluxOut << "mtllib " << Models.at(modelsNumber.at(n)).mtllib <<endl<<endl;

            else if(!Models.at(modelsNumber.at(n)).textureName.isEmpty())
            {
                QStringList lineList = Models.at(modelsNumber.at(n)).textureName.split('/');
                QFile texture(Models.at(modelsNumber.at(n)).textureName);
                texture.copy(newPath + lineList.last());

                QStringList nameList = Models.at(modelsNumber.at(n)).modelName.split('/');
                QString name = nameList.last().split('.').at(0) + QString(".mtl");

                QFile newMtllibFile(name);

                if (!newMtllibFile.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

                QTextStream flux(&newMtllibFile);
                flux.setCodec("UTF-8");
                flux << "map_Kd " << lineList.last();

                newMtllibFile.close();

                fluxOut << "mtllib " << name <<endl<<endl;
            }

            while(!fileText.atEnd())
            {
                QString fileLine = fileText.readLine();
                fileLine = fileLine.simplified();

            /* ============================ BODY - VERTICES ============================ */

                QMatrix4x4 r;
                r.rotate(Models.at(modelsNumber.at(n)).rotation);

                if(fileLine.startsWith("v "))
                {
                    QStringList lineList = fileLine.split(" ");
                    QVector3D coords(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());

                    if(Models.at(modelsNumber.at(n)).tumorRadius > 0)
                        coords = r * (coords*Models.at(modelsNumber.at(n)).tumorRadius/0.33 - Models.at(modelsNumber.at(n)).origin) + Models.at(modelsNumber.at(n)).position;
                    else
                        coords = r * (coords - Models.at(modelsNumber.at(n)).origin) + Models.at(modelsNumber.at(n)).position;

                    fluxOut << "v " << QString::number(coords.x(), 'f', 6)
                            << " " << QString::number(coords.y(), 'f', 6)
                            << " " << QString::number(coords.z(), 'f', 6) <<endl;
                }


            /* ============================ BODY - NORMAL ============================ */

                if(Models.at(modelsNumber.at(n)).vnFile)
                    if(fileLine.startsWith("vn "))
                    {
                        QStringList lineList = fileLine.split(" ");
                        QVector3D coords(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());

                        coords = r * (coords);

                        fluxOut << "vn " << QString::number(coords.x(), 'f', 6)
                                << " " << QString::number(coords.y(), 'f', 6)
                                << " " << QString::number(coords.z(), 'f', 6) <<endl;
                    }


            /* ============================ BODY - TEXTURE ============================ */

                if(Models.at(modelsNumber.at(n)).vtFile)
                    if(fileLine.startsWith("vt "))
                    {
                        QStringList lineList = fileLine.split(" ");
                        fluxOut << "vt " << lineList[1] << " " << lineList[2] <<endl;
                    }


            /* ============================ BODY - FACES ============================ */

                if(fileLine.startsWith("f "))
                {
                    QStringList lineList = fileLine.split(" ");

                    fluxOut << "f " << lineList[1] <<" "<< lineList[2] <<" "<< lineList[3];

                    if(lineList.size() > 4)
                        for(GLuint i = 4; i < (GLuint)lineList.size(); i++)
                            fluxOut<<" "<< lineList[i];

                    fluxOut<<endl;
                }
            }
            file.close();

            /* ============================ FOOTER ============================ */
            fluxOut << "\n# End of File ";
            newFile.close();


            if(!Models.at(modelsNumber.at(n)).mtllib.isEmpty())
            {
                QFile mtllibFile(path + Models.at(modelsNumber.at(n)).mtllib);
                mtllibFile.copy(newPath + Models.at(modelsNumber.at(n)).mtllib);


                if(mtllibFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream fileText(&mtllibFile);
                    bool textureFound = false;

                    while(!fileText.atEnd() && !textureFound)
                    {
                        QString fileLine = fileText.readLine();
                        fileLine.simplified();

                        if(fileLine.contains("map_Kd"))
                        {
                            textureFound = true;
                            QStringList lineList = fileLine.split(" ");
                            QString textureName = path + lineList[fileLine.indexOf("map_Kd")+1];
                            while(textureName.contains("\\"))       // If necessary, replaces '\' with '/'
                                textureName.replace(textureName.indexOf("\\", 1),1,"/");
                            while(textureName.contains(".."))       // If necessary, removes ".."
                                textureName.remove(textureName.indexOf("..", 1),2);
                            while(textureName.contains("\\"))       // If necessary, replaces "//" with '/'
                                textureName.replace(textureName.indexOf("//", 1),2,"/");

                            QFile texture(textureName);
                            texture.copy(newPath + lineList[fileLine.indexOf("map_Kd")+1]);
                        }
                    }
                    mtllibFile.close();
                }
            }
        }
}
void GLmodel::removeModel(GLuint modelNumber)
{
    Models.remove(modelNumber);
}


/* ============================ TEXTURE ============================ */
void GLmodel::loadMTL(QString MTLPath, GLuint modelNumber)
{
    QFile file(MTLPath + Models.at(modelNumber).mtllib);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream fileText(&file);
        bool textureFound = false;

        while (!fileText.atEnd() && !textureFound)
        {
            QString fileLine = fileText.readLine();
            fileLine.simplified();

            if(fileLine.contains("map_Kd"))
            {
                textureFound = true;
                QStringList lineList = fileLine.split(" ");
                QString textureName = MTLPath + lineList[fileLine.indexOf("map_Kd")+1];
                while(textureName.contains("\\"))       // If necessary, replaces '\' with '/'
                    textureName.replace(textureName.indexOf("\\", 1),1,"/");
                while(textureName.contains(".."))       // If necessary, removes ".."
                    textureName.remove(textureName.indexOf("..", 1),2);
                while(textureName.contains("\\"))       // If necessary, replaces "//" with '/'
                    textureName.replace(textureName.indexOf("//", 1),2,"/");

                loadTexture(textureName, modelNumber);
            }
        }
    }
    file.close();
}
void GLmodel::loadTexture(QString textureName, GLuint modelNumber)
{
    QImage textureImg = QImage(textureName);
    GLuint currentTexture;

    textureImg = QGLWidget::convertToGLFormat(textureImg);
    glGenTextures(1, &currentTexture);
    glBindTexture(GL_TEXTURE_2D, currentTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImg.width(), textureImg.height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, textureImg.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    Models[modelNumber].textureName = textureName;
    Models[modelNumber].texture[0] = currentTexture;
}


/* ============================ OPENGL TASKS ============================ */
void GLmodel::calculateNormal(GLuint i, GLuint modelNumber)
{
    if(!Models.at(modelNumber).vnFile)
        glNormal3f(
            (Faces[i].v[1].y()-Faces[i].v[0].y())*(Faces[i].v[2].z()-Faces[i].v[0].z())-(Faces[i].v[1].z()-Faces[i].v[0].z())*(Faces[i].v[2].y()-Faces[i].v[0].y()),
            (Faces[i].v[1].z()-Faces[i].v[0].z())*(Faces[i].v[2].x()-Faces[i].v[0].x())-(Faces[i].v[1].x()-Faces[i].v[0].x())*(Faces[i].v[2].z()-Faces[i].v[0].z()),
            (Faces[i].v[1].x()-Faces[i].v[0].x())*(Faces[i].v[2].y()-Faces[i].v[0].y())-(Faces[i].v[1].y()-Faces[i].v[0].y())*(Faces[i].v[2].x()-Faces[i].v[0].x())
                  );
}
void GLmodel::drawFace(GLuint i, GLuint j, GLuint modelNumber)
{
    if(Models.at(modelNumber).vnFile)   // Drawing normals
        glNormal3f(Faces[i].vn[j].x(), Faces[i].vn[j].y(), Faces[i].vn[j].z());

    if(Models.at(modelNumber).vtFile)   // Drawing texture coordinates
        glTexCoord2f(Faces[i].t[j].x(), Faces[i].t[j].y());
    else
        switch(j)
        {
            case 0:
                glTexCoord2i(0, 0);
                break;
            case 1:
                glTexCoord2i(1, 0);
                break;
            case 2:
                glTexCoord2i(1, 1);
                break;
            case 3:
                glTexCoord2i(0, 1);
                break;
        }

    glVertex3f(Faces[i].v[j].x()-Models.at(modelNumber).position.x(),   // Drawing vertices
               Faces[i].v[j].y()-Models.at(modelNumber).position.y(),
               Faces[i].v[j].z()-Models.at(modelNumber).position.z());
}


/* ============================ GETTERS ============================ */
Model GLmodel::getModelSettings(GLuint modelNumber)
{
    return Models.at(modelNumber);
}
GLuint GLmodel::getModelSize()
{
    return Models.size();
}


/* ============================ SETTERS ============================ */
void GLmodel::setModelSettings(Model newModel, GLuint modelNumber)
{
    Models[modelNumber] = newModel;
}
