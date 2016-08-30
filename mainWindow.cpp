#include "mainWindow.h"


MainWindow::MainWindow()
{
    screenshotNumber = 1;       // Number of screenshots taken

    QWidget *widgetPrincipal = new QWidget; // Main widget containing the main layout
    setCentralWidget(widgetPrincipal);

    layoutPrincipal = new QGridLayout(widgetPrincipal); // Main layout containing the OpenGLWidget and the toolbars
    widgetPrincipal->setLayout(layoutPrincipal);

    object = new OpenGLWidget;              // Creation of an OpenGLWidget object
    layoutPrincipal->addWidget(object);
    layoutPrincipal->setMargin(0);



// LEFT TOOLBAR BUTTONS

    leftToolBar = addToolBar("Left Toolbar");
    leftToolBar->setIconSize(QSize(40,40));

    // LOAD IMAGE
    QAction *actionLoadImage = new QAction(QIcon("./img/picture.jpg"), "Load &Image", this);
    leftToolBar->addAction(actionLoadImage);
    leftToolBar->addSeparator();

    // LOAD VIDEO
    QAction *actionLoadVideo = new QAction(QIcon("./img/camera.png"), "Load &Video", this);
    leftToolBar->addAction(actionLoadVideo);
    leftToolBar->addSeparator();

    // LOAD MODEL
    QAction *actionLoadModel = new QAction(QIcon("./img/load.png"), "&Load Model", this);
    leftToolBar->addAction(actionLoadModel);
    leftToolBar->addSeparator();

    // LOAD TEXTURE
    QAction *actionLoadTexture = new QAction(QIcon("./img/texture.png"), "Load T&exture", this);
    leftToolBar->addAction(actionLoadTexture);
    leftToolBar->addSeparator();

    // CHANGE COLOR
    QAction *actionChangeColor = new QAction(QIcon("./img/color.png"), "C&hange Color", this);
    leftToolBar->addAction(actionChangeColor);
    leftToolBar->addSeparator();

    // SAVE MODEL
    QAction *actionSaveModel = new QAction(QIcon("./img/save.png"), "Save &Model", this);
    leftToolBar->addAction(actionSaveModel);
    leftToolBar->addSeparator();

    // ADD TUMOR
    QAction *actionTumor = new QAction(QIcon("./img/tumor.png"), "Add &Tumor", this);
    leftToolBar->addAction(actionTumor);
    leftToolBar->addSeparator();

    // CENTER MODEL
    QAction *actionCenterModel = new QAction(QIcon("./img/center.png"), "&Center Model", this);
    leftToolBar->addAction(actionCenterModel);
    leftToolBar->addSeparator();

    // DISTANCE
    distance = new QAction(QIcon("./img/distance.png"), "&Distance", this);
    distance->setCheckable(true);
    leftToolBar->addAction(distance);
    leftToolBar->addSeparator();

    // ROTATE X
    QAction *rotateX = new QAction(QIcon("./img/rotateX.png"), "Rotate Model (&X Axis)", this);
    leftToolBar->addAction(rotateX);
    leftToolBar->addSeparator();

    // ROTATE Y
    QAction *rotateY = new QAction(QIcon("./img/rotateY.png"), "Rotate Model (&Y Axis)", this);
    leftToolBar->addAction(rotateY);
    leftToolBar->addSeparator();

    // SCREENSHOT
    QAction *actionScreenshot = new QAction(QIcon("./img/screenshot.png"), "Sc&reenshot", this);
    leftToolBar->addAction(actionScreenshot);
    leftToolBar->addSeparator();

    // FRAME BY FRAME
    frameByFrame = new QAction(QIcon("./img/frameByFrame.png"), "&Frame By Frame Video", this);
    frameByFrame->setCheckable(true);
    leftToolBar->addAction(frameByFrame);
    leftToolBar->addSeparator();

    // SETTINGS
    QAction *settings = new QAction(QIcon("./img/settings.png"), "&Settings", this);
    leftToolBar->addAction(settings);
    leftToolBar->addSeparator();

    addToolBar(Qt::LeftToolBarArea, leftToolBar);




// RIGHT TOOLBAR ELEMENTS
    rightToolBar = addToolBar("Right Toolbar");
    const QSize sliderSize(60,100);

    QWidget *slidersWidget = new QWidget(rightToolBar);     // Widget containing the right toolbar layout
    QVBoxLayout *slidersLayout = new QVBoxLayout(slidersWidget);    // Layout containing the right toolbar elements
    slidersLayout->setAlignment(Qt::AlignCenter);


    // OPACITY SLIDER

      // SLIDER TITLE
        QLabel *opacityTitle = new QLabel("Opacity");
        opacityTitle->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(opacityTitle);

      // SLIDER TEXTMAX
        QLabel *opacityTextMax = new QLabel("100%");
        opacityTextMax->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(opacityTextMax);

      // SLIDER
        opacitySlider = new QSlider;
        opacitySlider->setRange(0,100);
        opacitySlider->setSliderPosition(100);
        opacitySlider->setMaximumSize(sliderSize);
        slidersLayout->addWidget(opacitySlider);

      // SLIDER TEXTMIN
        QLabel *opacityTextMin = new QLabel("0%");
        opacityTextMin->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(opacityTextMin);


    // SCALE SLIDER

      // SLIDER TITLE
        QLabel *scaleTitle = new QLabel("Scale");
        scaleTitle->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(scaleTitle);

      // SLIDER TEXTMAX
        QLabel *scaleTextMax = new QLabel("2");
        scaleTextMax->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(scaleTextMax);

      // SLIDER
        scaleSlider = new QSlider;
        scaleSlider->setRange(1,200);
        scaleSlider->setSliderPosition(100);
        scaleSlider->setSingleStep(10);
        scaleSlider->setMaximumSize(sliderSize);
        slidersLayout->addWidget(scaleSlider);

      // SLIDER TEXTMIN
        QLabel *scaleTextMin = new QLabel("0");
        scaleTextMin->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(scaleTextMin);


        rightToolBar->addWidget(slidersWidget);
        rightToolBar->addSeparator();


    // MODELS LIST
      // MODELS LIST TITLE
        QLabel *modelsListTitle = new QLabel("Models List");
        modelsListTitle->setAlignment(Qt::AlignCenter);
        rightToolBar->addWidget(modelsListTitle);

      // MODELS LIST
        list = new ModelsListWidget;    // Creation of a ModelsListWidget object
        list->setFixedWidth(170);
        rightToolBar->addWidget(list);


    addToolBar(Qt::RightToolBarArea, rightToolBar);



    /* ============================ SIGNAL-SLOT CONNECTIONS ============================ */

// AUTO-RESIZE
    connect(object, SIGNAL(pictureChanged(GLuint,GLuint)), this, SLOT(resizeMainWindow(GLuint,GLuint)));


// LEFT TOOLBAR BUTTONS
    connect(actionLoadImage, SIGNAL(triggered()), object, SLOT(setTexturePath()));
    connect(actionLoadVideo, SIGNAL(triggered()), this, SLOT(setVideoPath()));
    connect(actionLoadModel, SIGNAL(triggered()), this, SLOT(addModel()));
    connect(actionLoadTexture, SIGNAL(triggered()), object, SLOT(addTexture()));
    connect(actionChangeColor, SIGNAL(triggered()), list, SLOT(emitChangeColor()));

    connect(actionSaveModel, SIGNAL(triggered()), object, SLOT(saveModels()));

    connect(actionTumor, SIGNAL(triggered()), object, SLOT(createTumor()));

    connect(actionCenterModel, SIGNAL(triggered()), object, SLOT(centerModels()));

    connect(distance, SIGNAL(changed()), this, SLOT(distanceMode()));
    connect(object, SIGNAL(distanceModeIsON(bool)), distance, SLOT(setChecked(bool)));

    connect(rotateX, SIGNAL(triggered()), object, SLOT(rotateX()));
    connect(rotateY, SIGNAL(triggered()), object, SLOT(rotateY()));

    connect(actionScreenshot, SIGNAL(triggered()), this, SLOT(takeScreenshot()));

    connect(frameByFrame, SIGNAL(changed()), this, SLOT(frameByFrameMode()));
    connect(object, SIGNAL(frameByFrameModeOFF(bool)), frameByFrame, SLOT(setChecked(bool)));
    connect(object, SIGNAL(takeScreenShot(QString)), this, SLOT(screenshot(QString)));

    connect(settings, SIGNAL(triggered()), this, SLOT(settingsWindow()));


// RIGHT TOOLBAR BUTTONS
  // OPACITY SLIDER
    connect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(updateToolTip(int)));
    connect(opacitySlider, SIGNAL(valueChanged(int)), object, SLOT(setOpacity(int)));
    connect(opacitySlider, SIGNAL(sliderReleased()), this, SLOT(focusOFF()));

  // SCALE SLIDER
    connect(scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(updateToolTip(int)));
    connect(scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(scaleSliderState()));
    connect(scaleSlider, SIGNAL(sliderReleased()), this, SLOT(scaleSliderState()));

  // MODELS LIST WIDGET
    connect(object, SIGNAL(modelsChanged()), this, SLOT(updateModelsList()));
    connect(list, SIGNAL(updateList()), this, SLOT(updateModelsList()));
    connect(list, SIGNAL(updateCheckedModels(QVector<GLuint>)), object, SLOT(setCheckedModels(QVector<GLuint>)));

    connect(list, SIGNAL(selectedModelChanged(QString)), object, SLOT(setSelectedModel(QString)));
    connect(list, SIGNAL(referenceModelChanged(QString)), object, SLOT(setReferenceModel(QString)));
    connect(list, SIGNAL(addModel()), this, SLOT(addModel()));
    connect(list, SIGNAL(saveModels()), object, SLOT(saveModels()));
    connect(list, SIGNAL(removeModels()), object, SLOT(removeModels()));
    connect(list, SIGNAL(modelColor(QColor)), object, SLOT(changeColor(QColor)));
    connect(list, SIGNAL(addTexture()), object, SLOT(addTexture()));
}


void MainWindow::resizeMainWindow(GLuint newWidth, GLuint newHeight)  // Resizes main window when OpenGLWidget size changes
{
    QDesktopWidget rec;
    QRect mainScreenSize = rec.availableGeometry(rec.primaryScreen());

    if((newHeight+20)<(GLuint)mainScreenSize.height())   // Test: The window cannot be larger than the screen
        this->setFixedHeight(newHeight+20);
    else
        this->setFixedHeight(mainScreenSize.height()-28);

    if(newWidth+rightToolBar->width()+leftToolBar->width()+20<(GLuint)mainScreenSize.width()) // Test: The window cannot be higher than the screen
        this->setFixedWidth(newWidth+rightToolBar->width()+leftToolBar->width()+20);
    else
        this->setFixedWidth(mainScreenSize.width());
}



/* ============================ LEFT TOOLBAR ============================ */
void MainWindow::distanceMode() // Send the state of the button "Distance Mode" to the OpenGLWidget
{
   if(distance->isChecked())
       object->setDistanceMode(true);
   else
       object->setDistanceMode(false);
}
void MainWindow::setVideoPath() // Calls the OpenGLWidget function setVideoPath(false) which indicates the "Frame By Frame Mode" is OFF
{
    object->setVideoPath(false);
}
void MainWindow::frameByFrameMode() // Send the state of the button "Frame By Frame Mode" to the OpenGLWidget
{
   if(frameByFrame->isChecked())
       object->setFrameByFrameMode(true);
   else
       object->setFrameByFrameMode(false);
}
void MainWindow::addModel() // Calls the OpenGLWidget function addModel with an empty QString for the user to select a file to open
{
    object->addModel(QString(""));
}

void MainWindow::takeScreenshot() // Takes a screenshot without specifying a folder
{
    screenshot(QString(""));
}
void MainWindow::screenshot(QString path) // Takes a screenshot in the selected folder
{
    QImage image = object->grabFrameBuffer();
    QString fileName;
    QString format = "png";

    if(path.isEmpty())  // If the argument is empty, the user has to chose a folder to save the screenshot into
        fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                   QString("screenshot%1.").arg(screenshotNumber) + format,
                                   QString("%1 Files (*.%2);;All Files (*)")
                                   .arg(format.toUpper())
                                   .arg(format));
    else
        fileName = path + QString("/screenshot%1.%2").arg(screenshotNumber).arg(format);

    if(fileName.isEmpty())
        return;

    image.save(fileName, qPrintable(format));
    screenshotNumber++;

}

void MainWindow::settingsWindow()   // Opens the settings window
{
    QDialog *settings = new QDialog(this);
    settings->setWindowTitle("Settings");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settings);
    QTabWidget *tabs = new QTabWidget(settings);
    QDialogButtonBox *saveQuitButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    settingsLayout->addWidget(tabs);
    settingsLayout->addWidget(saveQuitButtons);

    settings->setLayout(settingsLayout);


// Tabs
    QWidget *display = new QWidget;
    QWidget *camera = new QWidget;
    QWidget *sensibility = new QWidget;


    // DISPLAY
        framePictureRatioLineEdit = new QLineEdit(QString("%1").arg(object->getFramePictureRatio()));
        rotationSpeedLineEdit = new QLineEdit(QString("%1").arg(object->getRotationSpeed()));
        tagsRadiusLineEdit = new QLineEdit(QString("%1").arg(object->getTagsRadius()));
        tumorRadiusLineEdit = new QLineEdit(QString("%1").arg(object->getTumorRadius()));
        tumorDepthLineEdit = new QLineEdit(QString("%1").arg(object->getTumorDepth()));

        QLabel *rotationSpeed = new QLabel("Speed of the rotation when the buttons Rotate Model \n(X or Y Axis) are clicked.");
        QLabel *tagsRadius = new QLabel("Radius of the spheres created in Distance Mode.");
        QLabel *tumorRadius = new QLabel("Initial radius of the tumors.");
        QLabel *tumorDepth = new QLabel("Initial depth of the tumors.");

        QFormLayout *formLayout1 = new QFormLayout;


        formLayout1->addRow("&Ratio Frame/Picture:", framePictureRatioLineEdit);

        formLayout1->addRow("Rotation &Speed (°/s):", rotationSpeedLineEdit);
        formLayout1->addRow(rotationSpeed);

        formLayout1->addRow("&Tags Radius (m):", tagsRadiusLineEdit);
        formLayout1->addRow(tagsRadius);

        formLayout1->addRow("T&umors Radius (m):", tumorRadiusLineEdit);
        formLayout1->addRow(tumorRadius);

        formLayout1->addRow("Tumors &Depth (m):", tumorDepthLineEdit);
        formLayout1->addRow(tumorDepth);


        display->setLayout(formLayout1);


    // CAMERA
      // GRID LAYOUT
        QString zeroString = "0";

        QLabel *k = new QLabel("K");
        QLabel *equal = new QLabel("=");
        QLabel *zero1 = new QLabel(zeroString);
        QLabel *zero2 = new QLabel(zeroString);
        QLabel *zero3 = new QLabel(zeroString);
        QLabel *zero4 = new QLabel(zeroString);
        QLabel *zero5 = new QLabel(zeroString);
        QLabel *zero6 = new QLabel(zeroString);
        QLabel *zero7 = new QLabel(zeroString);
        QLabel *zero8 = new QLabel(zeroString);
        QLabel *a = new QLabel("A");
        QLabel *b = new QLabel("B");
        QLabel *minusOne = new QLabel("-1");

        alphaX = new QLineEdit(QString("%1").arg(object->getCameraSettings(0)));
        alphaY = new QLineEdit(QString("%1").arg(object->getCameraSettings(1)));
        skewness = new QLineEdit(QString("%1").arg(object->getCameraSettings(2)));
        centerX = new QLineEdit(QString("%1").arg(object->getCameraSettings(3)));
        centerY = new QLineEdit(QString("%1").arg(object->getCameraSettings(4)));

        alphaX->setFixedWidth(70);
        alphaY->setFixedWidth(70);
        skewness->setFixedWidth(70);
        centerX->setFixedWidth(70);
        centerY->setFixedWidth(70);

        QGridLayout *gridLayout = new QGridLayout;
        gridLayout->addWidget(alphaX,0,2);
        gridLayout->addWidget(skewness,0,3);
        gridLayout->addWidget(centerX,0,4);
        gridLayout->addWidget(zero1,0,5);
        gridLayout->addWidget(zero2,1,2);
        gridLayout->addWidget(alphaY,1,3);
        gridLayout->addWidget(centerY,1,4);
        gridLayout->addWidget(zero3,1,5);
        gridLayout->addWidget(k,2,0);
        gridLayout->addWidget(equal,2,1);
        gridLayout->addWidget(zero4,2,2);
        gridLayout->addWidget(zero5,2,3);
        gridLayout->addWidget(a,2,4);
        gridLayout->addWidget(b,2,5);
        gridLayout->addWidget(zero6,3,2);
        gridLayout->addWidget(zero7,3,3);
        gridLayout->addWidget(minusOne,3,4);
        gridLayout->addWidget(zero8,3,5);


      // TEXT LAYOUT
        QLabel *A = new QLabel("\nA = near + far");
        QLabel *B = new QLabel("B = near * far\n");
        QVBoxLayout *vTextLayout = new QVBoxLayout;
        vTextLayout->addWidget(A);
        vTextLayout->addWidget(B);


      // FORM LAYOUT
        near = new QLineEdit(QString("%1").arg(object->getCameraSettings(5)));
        far = new QLineEdit(QString("%1").arg(object->getCameraSettings(6)));

        QFormLayout *formLayout2 = new QFormLayout;
        formLayout2->addRow("&Near:", near);
        formLayout2->addRow("&Far:", far);


      // MAIN LAYOUT
        QVBoxLayout *vBoxLayout = new QVBoxLayout;
        vBoxLayout->addLayout(gridLayout);
        vBoxLayout->addLayout(vTextLayout);
        vBoxLayout->addLayout(formLayout2);


        camera->setLayout(vBoxLayout);

    // SENSIBILITY
        sensibilityLineEdit = new QLineEdit(QString("%1").arg(object->getSensibility()));
        sensibilityPlusLineEdit = new QLineEdit(QString("%1").arg(object->getSensibilityPlus()));
        QFormLayout *formLayout3 = new QFormLayout;

        QLabel *sens = new QLabel("Amplitude of the translation (Arrow keys or mouse wheel).");
        QLabel *sensPlus = new QLabel("Amplitude of the fine translation (Ctrl + arrow keys\nor mouse wheel).");

        formLayout3->addRow("&Displacement (m):", sensibilityLineEdit);
        formLayout3->addRow(sens);

        formLayout3->addRow("Displacement &Precision (m):", sensibilityPlusLineEdit);
        formLayout3->addRow(sensPlus);

        sensibility->setLayout(formLayout3);


    tabs->addTab(display, "Display");
    tabs->addTab(camera, "Camera");
    tabs->addTab(sensibility, "Displacement");
    tabs->adjustSize();


    connect(saveQuitButtons, SIGNAL(accepted()), settings, SLOT(accept()));
    connect(saveQuitButtons, SIGNAL(rejected()), settings, SLOT(reject()));

    if(settings->exec() == QDialog::Accepted)
        sendSettings();
}
void MainWindow::sendSettings() // Applies the new settings from the settings window
{
  // DISPLAY
    object->setFramePictureRatio(framePictureRatioLineEdit->text().toFloat());
    object->setRotationSpeed(rotationSpeedLineEdit->text().toFloat());
    object->setTagsRadius(tagsRadiusLineEdit->text().toFloat());
    object->setTumorRadius(tumorRadiusLineEdit->text().toFloat());
    object->setTumorDepth(tumorDepthLineEdit->text().toFloat());

  // CAMERA
    object->setCameraSettings(0, alphaX->text().toFloat());
    object->setCameraSettings(1, alphaY->text().toFloat());
    object->setCameraSettings(2, skewness->text().toFloat());
    object->setCameraSettings(3, centerX->text().toFloat());
    object->setCameraSettings(4, centerY->text().toFloat());
    object->setCameraSettings(5, near->text().toFloat());
    object->setCameraSettings(6, far->text().toFloat());

  // SENSIBILITY
    object->setSensibility(sensibilityLineEdit->text().toFloat());
    object->setSensibilityPlus(sensibilityPlusLineEdit->text().toFloat());
}



/* ============================ RIGHT TOOLBAR ============================ */
void MainWindow::updateToolTip(int sliderValue)     // Updates opacity tooltip
{
    QToolTip::showText(QCursor::pos(),QString("%1%").arg(sliderValue));
}
void MainWindow::scaleSliderState() // Send the sale slider state and value to the OpenGLWidget object
{
    object->scaleSliderState(scaleSlider->isSliderDown());
    object->setCameraSettings(7, (GLfloat)scaleSlider->value()/100);
    focusOFF();
}

void MainWindow::updateModelsList() // Updates the models list
{
    list->updateModelsList(object->getModelsList());
}

void MainWindow::focusOFF()     // Set keyboard focus onto the OpenGLWidget
{
    object->setFocus();
}
