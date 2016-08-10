#include "openGLWidget.h"


/* ============================ INITIALIZATION ============================ */
OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);             // Keyboard ON
    setCursor(Qt::PointingHandCursor);           // Cursor shape

    scaleFactor = 0.001;                         // Millimeters to meters conversion
    frame_picture_Ratio = 0.1;                   // Size of the black frame relative to image size
    sensibility = 0.01, sensibilityPlus = 0.001; // Sensibility of the translation (m)
    rotationSpeed = 3;
    referenceModel = -1;

    tumorMode = false;                           // "Add/Move Tumor" mode OFF
    tumor = 0;                                   // Tumor OFF
    tumorRadius = 0.01;                          // Radius of the tumor (m)
    coordTumor = QVector3D(0,0,0);               // Tumor translation

    crosshair = tags = 0;                        // Crosshair, tags OFF
    tagsRadius = 0.001;                          // Tags radius

    distanceMode = false;                        // Distance mode OFF
    distanceBetweenTags = 0;

    scaleSliderPressed = false;                  // Scale Slider OFF
    cameraParameters[7] = 0.85;                  // Relative scale of the perspective view
}
void OpenGLWidget::initializeGL()   // OPENGL SPACE INITIALIZATION
{
    glEnable(GL_BLEND);                                 // Opacity ON
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Opacity parameters

  // Default texture initialization
    backgroundTexture.setTexture(QString("./img/standardPicture1.jpg"));
    resizeWidget();

  // Picture's QUAD creation in a displaylist
    picture = glGenLists(1);
    glNewList(picture, GL_COMPILE);
        glBegin(GL_TRIANGLES);
           glTexCoord2i(0, 0); glVertex2i(-1, -1);
           glTexCoord2i(1, 0); glVertex2i(1, -1);
           glTexCoord2i(0, 1); glVertex2i(-1, 1);

           glTexCoord2i(1, 0); glVertex2i(1, -1);
           glTexCoord2i(0, 1); glVertex2i(-1, 1);
           glTexCoord2i(1, 1); glVertex2i( 1, 1);
        glEnd();
    glEndList();

  // Default model initialization
    checkedModels.push_back(0);
    modelsList << "./img/modelRéduit50%.obj";
    model.loadModel(modelsList.at(0), 0);
    emit modelsChanged();
    resetCameraSettings();
}
void OpenGLWidget::resetCameraSettings()
{
/*        // DEFAULT
        cameraParameters[0] = 1000;   // alphaX, focal (px)
        cameraParameters[1] = 1000;   // alphaY, focal (px)
        cameraParameters[2] = 0;      // skewness
        cameraParameters[3] = 250;    // u, image center abscissa (px)
        cameraParameters[4] = 187.5;  // v, image center ordinate (px)

        // PICTURES N°1
        cameraParameters[0] = 3260.4904041680534;  // alphaX, focal (px)
        cameraParameters[1] = 3247.8133528848193;  // alphaY, focal (px)
        cameraParameters[2] = 9.1996636861168586;  // skewness
        cameraParameters[3] = 580.40611599810813;  // u, image center abscissa (px)
        cameraParameters[4] = 572.27194900491202;  // v, image center ordinate (px)*/

        // PICTURES N°2
      /*  cameraParameters[0] = 3215.0213146269689;  // alphaX, focal (px)
        cameraParameters[1] = 3227.1754390328997;  // alphaY, focal (px)
        cameraParameters[2] = 8.7179384749909108;  // skewness
        cameraParameters[3] = 483.46333094489626;  // u, image center abscissa (px)
        cameraParameters[4] = 472.53980666143559;  // v, image center ordinate (px)*/
/*
        // PICTURES N°3
        cameraParameters[0] = 3232.528030290006;   // alphaX, focal (px)
        cameraParameters[1] = 3248.4333523027763;  // alphaY, focal (px)
        cameraParameters[2] = 14.659490990087299;  // skewness
        cameraParameters[3] = 716.19859622776949;  // u, image center abscissa (px)
        cameraParameters[4] = 443.6688114873711;   // v, image center ordinate (px)
*/
        // PICTURES SURGERY
/*        cameraParameters[0] = 2093.88374;  // alphaX, focal (px)
        cameraParameters[1] = 2093.88374;  // alphaY, focal (px)
        cameraParameters[2] = 0.00000;     // skewness
        cameraParameters[3] = 959.50000;   // u, image center abscissa (px)
        cameraParameters[4] = 539.50000;   // v, image center ordinate (px)*/


    // PATIENT 2
        cameraParameters[0] = 945.26018;  // alphaX, focal (px)
        cameraParameters[1] = 945.26018;  // alphaY, focal (px)
        cameraParameters[2] = 0;  // skewness
        cameraParameters[3] = 639.5;  // u, image center abscissa (px)
        cameraParameters[4] = 359.5;  // v, image center ordinate (px)

        cameraParameters[5] = 0.001;       // near, distance to the nearer depth clipping plane (m)
        cameraParameters[6] = 1000;        // far, distance to the farther depth clipping plane (m)
}


/* ============================ DRAWING LOOP ============================ */
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Buffers cleared


    glMatrixMode(GL_PROJECTION);                        // Set projection matrix as current matrix
    glLoadIdentity();
    GLfloat scale = frame_picture_Ratio*cameraParameters[7];
    glViewport(backgroundTexture.getWidth()*scale, backgroundTexture.getHeight()*scale, // Drawable area in the widget
               (GLint)width()-(2*backgroundTexture.getWidth()*scale),
               (GLint)height()-(2*backgroundTexture.getHeight()*scale));



// BACKGROUND TEXTURE
    glMatrixMode(GL_MODELVIEW);                             // Set model matrix as current matrix
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);                                // Texture ON
        glBindTexture(GL_TEXTURE_2D, backgroundTexture.getTexture()); // Texture load
        glCallList(picture);
    glDisable(GL_TEXTURE_2D);                               // Texture OFF (enables color)



// PROJECTION CAMERA
    glViewport(0, 0, (GLint)width(), (GLint)height());
    glEnable(GL_DEPTH_TEST);                                // Depth buffer ON

    QMatrix4x4 perspective(cameraParameters[0]*cameraParameters[7], cameraParameters[2]*cameraParameters[7], -(cameraParameters[3]+backgroundTexture.getWidth()*frame_picture_Ratio)*cameraParameters[7], 0,
                           0, cameraParameters[1]*cameraParameters[7], -(cameraParameters[4]+backgroundTexture.getHeight()*frame_picture_Ratio)*cameraParameters[7], 0,
                           0, 0, (cameraParameters[5] + cameraParameters[6]), (cameraParameters[5] * cameraParameters[6]),
                           0, 0, -1, 0);

     glMatrixMode(GL_PROJECTION);
     glOrtho(0, (GLfloat)width(), 0, (GLfloat)height(), cameraParameters[5], cameraParameters[6]);        // Setting orthographic camera matrix

     multMatrix(perspective);             // Multiplication by perspective matrix



// LIGHT
    glMatrixMode(GL_MODELVIEW);

    static const GLfloat dir_light[4] = {0.2f, 0.2f, 0.5f, 1.f};    // Light vector

    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);   // Light only applied on the exposed sight
    glLightfv(GL_LIGHT0, GL_POSITION, dir_light);       // Setting light

    glEnable(GL_NORMALIZE);                             // Normalization vectors ON
    glEnable(GL_COLOR_MATERIAL);                        // Model color ON
    glEnable(GL_LIGHT0);                                // LIGHT0 ON
    glEnable(GL_LIGHTING);                              // Shadows ON



// SCALE
    glScalef(scaleFactor,scaleFactor,scaleFactor);

    Model reference;
    if(referenceModel >= 0)
        reference = model.getModelSettings(referenceModel);



// TAGS
    glPushMatrix();
        glColor3f(1.f, 1.f, 1.f);

        if(referenceModel >= 0)
        {
            glTranslatef(reference.position.x(), reference.position.y(), reference.position.z());

            QMatrix4x4 m;
            m.rotate(reference.rotation);
            multMatrix(m);

            glTranslatef(-reference.position.x(), -reference.position.y(), -reference.position.z());
        }

        glTranslatef(distanceCoordinates1.x(), distanceCoordinates1.y(), distanceCoordinates1.z());
        glCallList(tags);
    glPopMatrix();



// TUMOR
    glPushMatrix();
        glColor3f(0.f, 0.f, 1.f);

        if(referenceModel >= 0)
        {
            glTranslatef(reference.position.x(), reference.position.y(), reference.position.z());

            QMatrix4x4 m;
            m.rotate(reference.rotation);
            multMatrix(m);

            glTranslatef(-reference.position.x(), -reference.position.y(), -reference.position.z());
        }

        glTranslatef(coordTumor.x(),coordTumor.y(),coordTumor.z());
        glCallList(tumor);
    glPopMatrix();


// MODELS
    for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
    {
        Model currentModel = model.getModelSettings(modelNumber);
        glColor4f(currentModel.color.x(), currentModel.color.y(), currentModel.color.z(), currentModel.color.w());

        glPushMatrix();
            if(referenceModel >= 0 && (GLint)modelNumber != referenceModel)
            {
                glTranslatef(reference.position.x(), reference.position.y(), reference.position.z());

                QMatrix4x4 m;
                m.rotate(currentModel.rotation);
                multMatrix(m);

                glTranslatef(-reference.position.x(), -reference.position.y(), -reference.position.z());

                glTranslatef(currentModel.position.x(), currentModel.position.y(), currentModel.position.z());
            }
            else
            {
                glTranslatef(currentModel.position.x(), currentModel.position.y(), currentModel.position.z());

                QMatrix4x4 m;
                m.rotate(currentModel.rotation);
                multMatrix(m);
            }


            if(currentModel.texture[0] > 0)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, currentModel.texture[0]);
            }

            glCallList(currentModel.model);

            if(currentModel.texture[0] > 0)
                glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }



// CROSSHAIR
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glDisable(GL_DEPTH_TEST);                           // Depth buffer OFF
    glDisable(GL_LIGHTING);                             // Light OFF

    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);                                  // Negative color
    glCallList(crosshair);                              // Drawing crosshair
    glDisable(GL_COLOR_LOGIC_OP);



// TEXT
    QFont font("Lato Black", 16);
    if(checkedModels.size() == 1)
    {
        QFontMetrics fm(font);
        QString depth = QString("Depth: %1 cm").arg(QString::number(-model.getModelSettings(checkedModels.at(0)).position.z()/10, 'f', 2));
        renderText(width()-fm.width(depth, -1)-5, 25, depth, font);
    }

    if(distanceBetweenTags)
        renderText(10, 50, QString("Distance between tags: %1 cm").arg(QString::number(abs(distanceBetweenTags/10), 'f', 2)), font);

    if(crosshair)
    {
        renderText(10, 25, QString("X: %1    Y: %2    Z: %3 cm")
                .arg(QString::number(surfaceCoordinates.x()/10, 'f', 2))
                .arg(QString::number(surfaceCoordinates.y()/10, 'f', 2))
                .arg(QString::number(surfaceCoordinates.z()/10, 'f', 2)), font);

        if(tumor)
        {
            QVector3D temp = coordTumor;

            if(modelsList.size())
                temp += model.getModelSettings(0).position;

            GLfloat distanceToTumor = sqrt(pow(surfaceCoordinates.x()-temp.x(),2)
                    +pow(surfaceCoordinates.y()-temp.y(),2)
                    +pow(surfaceCoordinates.z()+temp.z(),2));

            renderText(10, height()-10, QString("Distance to tumor: %1 cm").arg(QString::number(abs((distanceToTumor/10)-(tumorRadius/scaleFactor/10)), 'f', 2)), font);
        }
        crosshair = 0;
    }
}



/* ============================ BACKGROUND LOADER ============================ */
void OpenGLWidget::resizeWidget()
{
    GLfloat frames = (2*frame_picture_Ratio)+1;
    GLfloat scale = cameraParameters[7]*frames;

    if(height()<width())
    {
        if(backgroundTexture.getHeight()*scale>300)
        {
            this->setFixedHeight(backgroundTexture.getHeight()*scale);
            this->setFixedWidth(backgroundTexture.getWidth()*scale);
        }
        else
        {
            this->setFixedWidth(300/((GLfloat)backgroundTexture.getHeight()/backgroundTexture.getWidth()));
            this->setFixedHeight(300);
            cameraParameters[7] = (GLfloat)300/backgroundTexture.getHeight()/frames;
        }
    }
    else
    {
        if(backgroundTexture.getWidth()*scale>300)
        {
            this->setFixedHeight(backgroundTexture.getHeight()*scale);
            this->setFixedWidth(backgroundTexture.getWidth()*scale);
        }
        else
        {
            this->setFixedWidth(300);
            this->setFixedHeight(300*((GLfloat)backgroundTexture.getHeight()/backgroundTexture.getWidth()));
            cameraParameters[7] = (GLfloat)300/backgroundTexture.getWidth()/frames;
        }
    }

    if(scaleSliderPressed==false)
        emit pictureChanged(width()-20, height()-20);
}
void OpenGLWidget::setTexturePath()
{
    releaseVideoCapture();

    QString texturePath = QFileDialog::getOpenFileName(this, "Open Image File", "./", "Images (*.png *.xpm *.jpg *.bmp)");
    if(!texturePath.isEmpty())
        backgroundTexture.setTexture(texturePath);

    resizeWidget();
    updateGL();
}

void OpenGLWidget::setVideoPath()
{
    releaseVideoCapture();

    QString videoPath = QFileDialog::getOpenFileName(this, "Open Video File", "./", "Videos (*.avi *.mpg *.mpeg...)");
    if(!videoPath.isEmpty())
    {
        cap = VideoCapture(videoPath.toStdString());

        backgroundTexture.setResizeWidget(true);
        bool temp = frameByFrameMode;
        frameByFrameMode = false;
        updateVideo();
        frameByFrameMode = temp;
        backgroundTexture.setResizeWidget(false);
        resizeWidget();

        if(!frameByFrameMode)
        {
            QTimer *timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(updateVideo()));
            connect(this, SIGNAL(captureReleased()), timer, SLOT(stop()));

            GLfloat fps = cap.get(CAP_PROP_FPS);
            timer->start(1000/fps);
        }
    }
}
void OpenGLWidget::updateVideo()
{
    if(frameByFrameMode)
        emit takeScreenShot(framesFolder);

    Mat frame;
    cap >> frame;                                       // Get a new frame from camera

    if(!frame.empty())
    {
        backgroundTexture.setTexture(frame);
        updateGL();
    }
    else
        releaseVideoCapture();
}
void OpenGLWidget::releaseVideoCapture()
{
    cap.release();
    emit captureReleased();
}
void OpenGLWidget::setFrameByFrameMode(bool buttonChecked)
{
    frameByFrameMode = buttonChecked;
    if(frameByFrameMode)
        setVideoPath();

    framesFolder = QFileDialog::getExistingDirectory(this, "Select a folder to save the frames", "/home",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}



/* ============================ MODEL LOADER/SAVER/TRANSFORM ============================ */
void OpenGLWidget::addModel()
{
    QString modelName;
    QStringList modelsListInit = modelsList;

    do
    {
        modelName = QFileDialog::getOpenFileName(this, tr("Open Model File"), "./", tr("Modèles 3D (*.obj)"));
        if(!modelName.isEmpty() && !modelsListInit.contains(modelName))
        {
            modelsList << modelName;
            model.loadModel(modelName, (GLuint)modelsList.size()-1);
        }

        if(modelsListInit.contains(modelName))
            QMessageBox::warning(this, "Model already loaded", "This model is already loaded. Please select a different model.");
    } while(modelsListInit.contains(modelName));

    updateModelsList();
    updateGL();
}
void OpenGLWidget::removeModels()
{
    if(!selectedModel.isEmpty())
    {
        GLuint modelNumber = modelsList.indexOf(selectedModel,0);
        modelsList.removeAt(modelNumber);
        model.removeModel(modelNumber);

        emit modelsChanged();
        updateGL();
    }
    else
    {
        for(GLuint i = (GLuint)checkedModels.size()-1; i <= 0; i--)
        {
            modelsList.removeAt(checkedModels.at(i));
            model.removeModel(checkedModels.at(i));

            emit modelsChanged();
            updateGL();
        }
        referenceModel = -1;
    }
}
void OpenGLWidget::saveModels()      // Saves the models with their transformations
{
    if(checkedModels.size() == 1 || !selectedModel.isEmpty())
    {
        QString newModelName;
        bool boucle = false;

        do{      // Checking if selected file is the source file
            if(boucle)
                QMessageBox::warning(this,"Source file selected!", "Please select a recording file different from the source file or create a new one.");
            boucle = true;
            newModelName = QFileDialog::getSaveFileName(this, "Save File", "./", "3D Object (*.obj)");
        }while(modelsList.contains(newModelName) && !newModelName.isEmpty());

        if(newModelName.isEmpty())
            return;

        QStringList newModelsNames;
        newModelsNames << newModelName;

        if(checkedModels.size() == 1)
            model.saveModel(newModelsNames, checkedModels, false);
        else
        {
            QVector<GLuint> selectVector;
            selectVector.push_back(modelsList.indexOf(selectedModel,0));
            model.saveModel(newModelsNames, selectVector, false);
        }
    }

    else if(!checkedModels.size())
        QMessageBox::warning(this, "Select a model to save", "No model has been selected. Please select a model to save.");

    else
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setWindowTitle("Assemble models?");
        messageBox.setText("Several models have been selected. Do you want to assemble them in one file? If you select No, they will be saved in different files.");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        if(messageBox.exec() == 0x00004000)
        {
            QString newModelName;
            bool boucle = false;

            do      // Checking if selected file is the source file
            {
              if(boucle)
                   QMessageBox::warning(this,"Source file selected!", "Please select a recording file different from the source file or create a new one.");
              boucle = true;
              newModelName = QFileDialog::getSaveFileName(this, "Save File", "./", "3D Object (*.obj)");
            }while(modelsList.contains(newModelName) && !newModelName.isEmpty());

            if(!newModelName.isEmpty())
            {
                QStringList newModelsNames;
                newModelsNames << newModelName;

                model.saveModel(newModelsNames, checkedModels, true);
            }
        }

        else
        {
            QStringList newModelsNames;

            for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
                if(checkedModels.contains(modelNumber))
                    {
                        QString newModelName;
                        bool boucle = false;

                        do      // Checking if selected file is the source file
                        {
                          if(boucle)
                               QMessageBox::warning(this,"Source file selected!", "Please select a recording file different from the source file or create a new one.");
                          boucle = true;
                          newModelName = QFileDialog::getSaveFileName(this, QString("Save List Model Number %1").arg(modelNumber), "./", "3D Object (*.obj)");
                        }while(modelsList.contains(newModelName) || newModelsNames.contains(newModelName));

                        if(!newModelName.isEmpty())
                            newModelsNames << newModelName;
                    }

            model.saveModel(newModelsNames, checkedModels, false);
        }
    }
}
void OpenGLWidget::changeColor(QColor newColor)
{
    if(!selectedModel.isEmpty())
    {
        GLuint modelNumber = modelsList.indexOf(selectedModel,0);
        Model currentModel = model.getModelSettings(modelNumber);
        currentModel.color.setX(newColor.redF());
        currentModel.color.setY(newColor.greenF());
        currentModel.color.setZ(newColor.blueF());
        model.setModelSettings(currentModel, modelNumber);
    }
    else if(checkedModels.size())
        for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
        {
            Model currentModel = model.getModelSettings(checkedModels.at(i));
            currentModel.color.setX(newColor.redF());
            currentModel.color.setY(newColor.greenF());
            currentModel.color.setZ(newColor.blueF());
            model.setModelSettings(currentModel, checkedModels.at(i));
        }

    updateGL();
}

void OpenGLWidget::updateModelsList()
{
    modelsList.clear();

    for(GLuint i = 0; i < model.getModelSize(); i++)
        modelsList << model.getModelSettings(i).modelName;

    emit modelsChanged();
}

void OpenGLWidget::addTexture()
{
    QString texturePath = QFileDialog::getOpenFileName(this, "Open Image File", "./", "Images (*.png *.xpm *.jpg *.bmp)");
    if(!texturePath.isEmpty())
    {
        if(!selectedModel.isEmpty())
        {
            GLuint modelNumber = modelsList.indexOf(selectedModel,0);
            Model currentModel = model.getModelSettings(modelNumber);

            currentModel.color = QVector4D(1.f, 1.f, 1.f, 1.f);
            model.setModelSettings(currentModel, modelNumber);
            model.loadTexture(texturePath, modelNumber);
        }
        else
            for(GLuint i = (GLuint)checkedModels.size()-1; i <= 0; i--)
            {
                GLuint modelNumber = checkedModels.at(i);
                Model currentModel = model.getModelSettings(modelNumber);

                currentModel.color = QVector4D(1.f, 1.f, 1.f, 1.f);
                model.setModelSettings(currentModel, modelNumber);
                model.loadTexture(texturePath, modelNumber);
            }

        updateGL();
    }
}

void OpenGLWidget::centerModels()    // Puts the center of the selected models on the widget center
{
    for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
        if(checkedModels.contains(modelNumber))
        {
            Model currentModel = model.getModelSettings(modelNumber);

            currentModel.position.setZ(-0.3/scaleFactor);

            currentModel.position.setY(((GLfloat)height()/2-(cameraParameters[4]+backgroundTexture.getHeight()*frame_picture_Ratio)*cameraParameters[7])*(-currentModel.position.z())/(cameraParameters[1]*cameraParameters[7]));
            currentModel.position.setX(((GLfloat)width()/2-currentModel.position.y()*cameraParameters[2]*cameraParameters[7]/(-currentModel.position.z())-(cameraParameters[3]+backgroundTexture.getWidth()*frame_picture_Ratio)*cameraParameters[7])*(-currentModel.position.z())/(cameraParameters[0]*cameraParameters[7]));

            currentModel.rotation = QQuaternion();

            model.setModelSettings(currentModel, modelNumber);
        }
    updateGL();
}


void OpenGLWidget::rotateX()         // Rotates the model at 360° around X axis
{
    QVector<QQuaternion> memory;
    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        memory.push_back(currentModel.rotation);
    }

    QVector<QVector3D> orthonormals;
    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        QMatrix4x4 m;
        m.rotate(currentModel.rotation);
        orthonormals.push_back(m.transposed() * QVector3D(1,0,0));
        model.setModelSettings(currentModel, checkedModels.at(i));
    }

    QTime time;
    time.start();

    while(time.elapsed() < 360.f*rotationSpeed)
    {
        for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
        {
            Model currentModel = model.getModelSettings(checkedModels.at(i));
            currentModel.rotation = memory.at(i) * QQuaternion::fromAxisAndAngle(orthonormals.at(i),-time.elapsed()/rotationSpeed);
            model.setModelSettings(currentModel, checkedModels.at(i));
        }
        updateGL();
    }

    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        currentModel.rotation = memory.at(i);
        model.setModelSettings(currentModel, checkedModels.at(i));
    }
    updateGL();
}
void OpenGLWidget::rotateY()         // Rotates the model at 360° around Y axis
{
    QVector<QQuaternion> memory;
    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        memory.push_back(currentModel.rotation);
    }

    QVector<QVector3D> orthonormals;
    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        QMatrix4x4 m;
        m.rotate(currentModel.rotation);
        orthonormals.push_back(m.transposed() * QVector3D(0,1,0));
        model.setModelSettings(currentModel, checkedModels.at(i));
    }

    QTime time;
    time.start();

    while(time.elapsed() < 360.f*rotationSpeed)
    {
        for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
        {
            Model currentModel = model.getModelSettings(checkedModels.at(i));
            currentModel.rotation = memory.at(i) * QQuaternion::fromAxisAndAngle(orthonormals.at(i),-time.elapsed()/rotationSpeed);
            model.setModelSettings(currentModel, checkedModels.at(i));
        }
        updateGL();
    }

    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        currentModel.rotation = memory.at(i);
        model.setModelSettings(currentModel, checkedModels.at(i));
    }
    updateGL();
}



/* ============================ OTHER MODELS ============================ */
void OpenGLWidget::createTumor(bool buttonChecked)
{
    tumorMode = buttonChecked;

    if(tumorMode)
    {
        distanceMode = false;
        emit distanceModeIsON(false);

        tumor = glGenLists(1);
        GLUquadric* params = gluNewQuadric();
        gluQuadricDrawStyle(params, GLU_FILL);

        glNewList(tumor, GL_COMPILE);
            gluSphere(params, tumorRadius/scaleFactor, 20, 20);
        glEndList();

        if(referenceModel >= 0)
            coordTumor = model.getModelSettings(referenceModel).position;
        else
            coordTumor = QVector3D(0,0,-0.3/scaleFactor);

        updateGL();
    }
}
void OpenGLWidget::createCrosshair(QPointF screenCoordinates)
{
    crosshair = glGenLists(1);
    glLineWidth(2);
    glNewList(crosshair, GL_COMPILE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width(), 0, height(), -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBegin(GL_LINES);
            glVertex2f(screenCoordinates.x() - 7, height()-screenCoordinates.y());
            glVertex2f(screenCoordinates.x() + 7, height()-screenCoordinates.y());
            glVertex2f(screenCoordinates.x(), height()-screenCoordinates.y() + 7);
            glVertex2f(screenCoordinates.x(), height()-screenCoordinates.y() - 7);
        glEnd();
    glEndList();

    updateGL();
}

void OpenGLWidget::setDistanceMode(bool buttonChecked)
{
    distanceMode = buttonChecked;

    if(distanceMode)
    {
        tumorMode = false;
        emit tumorModeIsON(false);
    }
}
void OpenGLWidget::createTags(QPointF screenCoordinates)
{
    if(!tags)
        distanceCoordinates1 = QVector3D(0,0,0);

    tags = glGenLists(1);
    GLUquadric* params = gluNewQuadric();
    gluQuadricDrawStyle(params, GLU_FILL);

    QMatrix4x4 m;
    if(referenceModel >= 0)
        m.rotate(model.getModelSettings(referenceModel).rotation);

    if(distanceCoordinates1.isNull())   // First tag
    {
        distanceBetweenTags = 0;
        distanceCoordinates1 = screenToModelPixel(screenCoordinates);
        distanceCoordinates1 = m.transposed() * QVector3D(distanceCoordinates1.x(), distanceCoordinates1.y(), -distanceCoordinates1.z());

        glNewList(tags, GL_COMPILE);
            gluSphere(params, tagsRadius/scaleFactor, 20, 20);
        glEndList();
    }
    else    // First and second tags
    {
        GLfloat radius = tagsRadius/scaleFactor;
        QVector3D distanceCoordinates2 = screenToModelPixel(screenCoordinates);
        distanceCoordinates2 = m.transposed() * QVector3D(distanceCoordinates2.x(), distanceCoordinates2.y(), -distanceCoordinates2.z());

        glNewList(tags, GL_COMPILE);
            gluSphere(params, radius, 20, 20);

            glTranslatef(-distanceCoordinates1.x(), -distanceCoordinates1.y(), -distanceCoordinates1.z());
            glTranslatef(distanceCoordinates2.x(), distanceCoordinates2.y(), distanceCoordinates2.z());

            gluSphere(params, radius, 20, 20);
        glEndList();

        distanceBetweenTags = sqrt(pow(distanceCoordinates1.x()-distanceCoordinates2.x(),2)
                +pow(distanceCoordinates1.y()-distanceCoordinates2.y(),2)
                +pow(distanceCoordinates1.z()-distanceCoordinates2.z(),2));

        distanceMode = false;
        emit distanceModeIsON(false);
    }

    updateGL();
}



/* ============================ MOUSE AND KEYBOARD TRANSFORMATIONS ============================ */
void OpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton && !checkedModels.isEmpty())
    {
        trackball.setRotation(QQuaternion());
        trackball.move(pixelPosToViewPos(e->localPos()));

        for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
            if(checkedModels.contains(modelNumber))
            {
                Model currentModel = model.getModelSettings(modelNumber);
                currentModel.rotation = trackball.rotation() * currentModel.rotation;
                model.setModelSettings(currentModel, modelNumber);
            }

        updateGL();
    }
}
void OpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton && !checkedModels.isEmpty())
    {
        updateGL();
        QPointF screenCoordinates = e->localPos();
        surfaceCoordinates = screenToModelPixel(screenCoordinates);

        createCrosshair(screenCoordinates);

        setCursor(Qt::ClosedHandCursor);

        trackball.push(pixelPosToViewPos(screenCoordinates));
    }
    else if(e->buttons() & Qt::RightButton && distanceMode)
        createTags(e->localPos());
}
void OpenGLWidget::mouseReleaseEvent(QMouseEvent*)
{
    setCursor(Qt::PointingHandCursor);
}

void OpenGLWidget::wheelEvent(QWheelEvent *e)
{
    GLfloat move;

    if(e->modifiers().testFlag(Qt::ControlModifier))
        move = sensibilityPlus;
    else
        move = sensibility;

    QVector3D movement = QVector3D(0,0,-(GLfloat)e->delta()/120*move/scaleFactor);

    if(tumorMode)
    {
        if(e->modifiers().testFlag(Qt::AltModifier))
        {
            tumorRadius += movement.z()/10000;

            QVector3D const tempCoordTumor = coordTumor;
            createTumor(true);
            coordTumor = tempCoordTumor;
        }

        else
        {
            if(referenceModel >= 0)
            {
                QMatrix4x4 m;
                m.rotate(model.getModelSettings(referenceModel).rotation);
                movement = m.transposed() * movement;
            }

            coordTumor = coordTumor + movement;
        }
    }
    else
        if(checkedModels.contains(referenceModel))
            for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
            {
                Model currentModel = model.getModelSettings(modelNumber);
                currentModel.position += movement;
                model.setModelSettings(currentModel, modelNumber);
            }
        else
            for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
                if(checkedModels.contains(modelNumber))
                {
                    QVector3D tempMovement = movement;

                    if(referenceModel >= 0 && referenceModel != (GLint)modelNumber)
                    {
                        QMatrix4x4 r;
                        r.rotate(model.getModelSettings(modelNumber).rotation);
                        tempMovement = r.transposed() * movement;
                    }

                    Model currentModel = model.getModelSettings(modelNumber);
                    currentModel.position += tempMovement;
                    model.setModelSettings(currentModel, modelNumber);
                }

        if(referenceModel >= 0 && checkedModels.contains(referenceModel))
            coordTumor = coordTumor + movement;

    updateGL();
}
void OpenGLWidget::keyPressEvent(QKeyEvent *e)
{
    GLfloat move = 0;
    QVector3D movement;

    if(e->modifiers() && Qt::ControlModifier)
        move = sensibilityPlus;
    else
        move = sensibility;

    switch(e->key())
    {
        case Qt::Key_Left:
            movement.setX(-move/scaleFactor);
            break;
        case Qt::Key_Right:
            movement.setX(move/scaleFactor);
            break;
        case Qt::Key_Down:
            movement.setY(-move/scaleFactor);
            break;
        case Qt::Key_Up:
            movement.setY(+move/scaleFactor);
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if(tumorMode)
                emit tumorModeIsON(false);
            else if(frameByFrameMode)
                updateVideo();
            break;
        case Qt::Key_N:
            if(frameByFrameMode)
            {
                frameByFrameMode = false;
                updateVideo();
                frameByFrameMode = true;
            }
            break;
        case Qt::Key_Escape:
            if(tumorMode)
            {
                tumorMode=false;
                emit tumorModeIsON(false);
                tumor = 0;
                coordTumor = QVector3D(0,0,0);
            }
            else if(distanceMode)
            {
                distanceMode=false;
                tags = 0;
                distanceBetweenTags = 0;
                emit distanceModeIsON(false);
            }
            else
                qApp->quit();
            break;
        default:
            break;
    }

    if(tumorMode)
    {
        if(referenceModel >= 0)
        {
            QMatrix4x4 m;
            m.rotate(model.getModelSettings(referenceModel).rotation);
            m = m.transposed();

            movement = m * movement;
        }

        coordTumor = coordTumor + movement;
    }
    else
    {
        if(checkedModels.contains(referenceModel))
            for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
            {
                Model currentModel = model.getModelSettings(modelNumber);
                currentModel.position += movement;
                model.setModelSettings(currentModel, modelNumber);
            }
        else
            for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
                if(checkedModels.contains(modelNumber))
                {
                    QVector3D tempMovement = movement;

                    if(referenceModel >= 0 && referenceModel != (GLint)modelNumber)
                    {
                        QMatrix4x4 r;
                        r.rotate(model.getModelSettings(modelNumber).rotation);
                        tempMovement = r.transposed() * movement;
                    }

                    Model currentModel = model.getModelSettings(modelNumber);
                    currentModel.position += tempMovement;
                    model.setModelSettings(currentModel, modelNumber);
                }

        if(referenceModel >= 0 && checkedModels.contains(referenceModel))
        {
            coordTumor += movement;
            distanceCoordinates1 += movement;
        }
    }

    updateGL();
}



/* ============================ OPERATORS ============================ */
void OpenGLWidget::multMatrix(const QMatrix4x4& m)    // Multiplies QMatrix4x4 as a GLfloat matrix
{
     static GLfloat mat[16];
     const GLfloat *data = m.constData();
     for (GLint index = 0; index < 16; ++index)
        mat[index] = data[index];
     glMultMatrixf(mat);
}
QPointF OpenGLWidget::pixelPosToViewPos(const QPointF& p)    // Sets pixel coordinates to view coordinates
{
    return QPointF(2.0*GLfloat(p.x())/width()-1.0, 1.0-2.0*GLfloat(p.y())/height());
}
QVector3D OpenGLWidget::screenToModelPixel(const QPointF screenCoordinates) // Sets 2D screen coordinates to 3D model coordinates
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels((float)screenCoordinates.x(), viewport[3]-screenCoordinates.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject((float)screenCoordinates.x(), (float)(viewport[3]-screenCoordinates.y()), winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    QVector3D pos(posX, posY, posZ);
    QMatrix4x4 r;
    r.rotate(trackball.rotation());
    pos = r * pos;

    return QVector3D(pos.x(),pos.y(),-pos.z());
}



/* ============================ GETTERS ============================ */
GLfloat OpenGLWidget::getFramePictureRatio()
{
    return frame_picture_Ratio;
}
qreal OpenGLWidget::getCameraSettings(GLuint settingNumber)
{
    return cameraParameters[settingNumber];
}
GLfloat OpenGLWidget::getSensibility()
{
    return sensibility;
}
GLfloat OpenGLWidget::getSensibilityPlus()
{
    return sensibilityPlus;
}
GLfloat OpenGLWidget::getRotationSpeed()
{
    return rotationSpeed;
}
GLfloat OpenGLWidget::getTagsRadius()
{
    return tagsRadius;
}
QStringList OpenGLWidget::getModelsList()
{
    return modelsList;
}



/* ============================ SETTERS ============================ */
void OpenGLWidget::setOpacity(GLint sliderValue)
{
    GLfloat opacity = ((GLfloat)sliderValue/100);
    for(GLuint i = 0; i < (GLuint)checkedModels.size(); i++)
    {
        Model currentModel = model.getModelSettings(checkedModels.at(i));
        currentModel.color.setW(opacity);
        model.setModelSettings(currentModel, checkedModels.at(i));
    }
    updateGL();
}
void OpenGLWidget::setFramePictureRatio(GLfloat new_frame_picture_Ratio)
{
    frame_picture_Ratio = new_frame_picture_Ratio;
    resizeWidget();
    updateGL();
}
void OpenGLWidget::setCameraSettings(GLuint settingNumber, qreal newValue)
{
    cameraParameters[settingNumber] = newValue;
    if(settingNumber==7)
        resizeWidget();
    updateGL();
}
void OpenGLWidget::setSensibility(GLfloat newValue)
{
    sensibility = newValue;
}
void OpenGLWidget::setSensibilityPlus(GLfloat newValue)
{
    sensibilityPlus = newValue;
}
void OpenGLWidget::setRotationSpeed(GLfloat newValue)
{
    rotationSpeed = newValue;
}
void OpenGLWidget::scaleSliderState(bool newState)
{
    scaleSliderPressed = newState;
}
void OpenGLWidget::setTagsRadius(GLfloat newValue)
{
    tagsRadius = newValue;
}

void OpenGLWidget::setCheckedModels(QVector<GLuint> newCheckedModels)
{
    checkedModels = newCheckedModels;
}
void OpenGLWidget::setSelectedModel(QString newSelectedModel)
{
    selectedModel = newSelectedModel;
}
void OpenGLWidget::setReferenceModel(QString newReferenceModel)
{
    if(newReferenceModel.isEmpty())
        referenceModel = -1;

    else
    for(GLuint modelNumber = 0; modelNumber < (GLuint)modelsList.size(); modelNumber++)
        if(modelsList.at(modelNumber).contains(newReferenceModel))
        {
            referenceModel = modelNumber;
            break;
        }
}
