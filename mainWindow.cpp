#include "mainWindow.h"


MainWindow::MainWindow()
{
    screenshotNumber = 1;       // Number of screenshots taken

    QWidget *widgetPrincipal = new QWidget;
    setCentralWidget(widgetPrincipal);

    layoutPrincipal = new QGridLayout(widgetPrincipal);
    widgetPrincipal->setLayout(layoutPrincipal);

    object = new OpenGLWidget;
    layoutPrincipal->addWidget(object);
    layoutPrincipal->setMargin(0);


// LEFT TOOLBAR

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

    // ADD/MOVE TUMOR
    actionTumor = new QAction(QIcon("./img/tumor.png"), "Add/Move &Tumor", this);
    actionTumor->setCheckable(true);
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


// RIGHT TOOLBAR
    rightToolBar = addToolBar("Right Toolbar");
    const QSize sliderSize(60,100);

    QWidget *slidersWidget = new QWidget(rightToolBar);
    QVBoxLayout *slidersLayout = new QVBoxLayout(slidersWidget);
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
        list = new ModelsListWidget;
        list->setFixedWidth(170);
        rightToolBar->addWidget(list);


    addToolBar(Qt::RightToolBarArea, rightToolBar);



    /* ============================ CONNECTIONS ============================ */

// AUTO-RESIZE
    connect(object, SIGNAL(pictureChanged(GLuint,GLuint)), this, SLOT(resizeMainWindow(GLuint,GLuint)));


// LEFT TOOLBAR BUTTONS
    connect(actionLoadImage, SIGNAL(triggered()), object, SLOT(setTexturePath()));
    connect(actionLoadVideo, SIGNAL(triggered()), object, SLOT(setVideoPath()));
    connect(actionLoadModel, SIGNAL(triggered()), object, SLOT(addModel()));
    connect(actionLoadTexture, SIGNAL(triggered()), object, SLOT(addTexture()));
    connect(actionChangeColor, SIGNAL(triggered()), list, SLOT(emitChangeColor()));

    connect(actionSaveModel, SIGNAL(triggered()), object, SLOT(saveModels()));

    connect(actionTumor, SIGNAL(changed()), this, SLOT(createTumor()));
    connect(object, SIGNAL(tumorModeIsON(bool)), actionTumor, SLOT(setChecked(bool)));

    connect(actionCenterModel, SIGNAL(triggered()), object, SLOT(centerModels()));

    connect(distance, SIGNAL(changed()), this, SLOT(distanceMode()));
    connect(object, SIGNAL(distanceModeIsON(bool)), distance, SLOT(setChecked(bool)));

    connect(rotateX, SIGNAL(triggered()), object, SLOT(rotateX()));
    connect(rotateY, SIGNAL(triggered()), object, SLOT(rotateY()));

    connect(actionScreenshot, SIGNAL(triggered()), this, SLOT(screenshot()));

    connect(frameByFrame, SIGNAL(changed()), this, SLOT(frameByFrameMode()));
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
    connect(list, SIGNAL(addModel()), object, SLOT(addModel()));
    connect(list, SIGNAL(saveModels()), object, SLOT(saveModels()));
    connect(list, SIGNAL(removeModels()), object, SLOT(removeModels()));
    connect(list, SIGNAL(changeColor(QColor)), object, SLOT(changeColor(QColor)));
    connect(list, SIGNAL(addTexture()), object, SLOT(addTexture()));
}


void MainWindow::resizeMainWindow(GLuint newWidth, GLuint newHeight)  // Resizes main window when openGLWidget size changes
{
    QDesktopWidget rec;
    QRect mainScreenSize = rec.availableGeometry(rec.primaryScreen());

    if((newHeight+20)<(GLuint)mainScreenSize.height())
        this->setFixedHeight(newHeight+20);
    else
        this->setFixedHeight(mainScreenSize.height()-28);

    if(newWidth+rightToolBar->width()+leftToolBar->width()+20<(GLuint)mainScreenSize.width())
        this->setFixedWidth(newWidth+rightToolBar->width()+leftToolBar->width()+20);
    else
        this->setFixedWidth(mainScreenSize.width());
}


/* ============================ LEFT TOOLBAR ============================ */
void MainWindow::createTumor()
{
   if(actionTumor->isChecked())
       object->createTumor(true);
   else
       object->createTumor(false);
}
void MainWindow::distanceMode()
{
   if(distance->isChecked())
       object->setDistanceMode(true);
   else
       object->setDistanceMode(false);
}
void MainWindow::frameByFrameMode()
{
   if(frameByFrame->isChecked())
       object->setFrameByFrameMode(true);
   else
       object->setFrameByFrameMode(false);
}

void MainWindow::screenshot()
{
    QImage image = object->grabFrameBuffer();

    QString format = "png";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               QString("screenshot%1.").arg(screenshotNumber) + format,
                               QString("%1 Files (*.%2);;All Files (*)")
                               .arg(format.toUpper())
                               .arg(format));
    if(!fileName.isEmpty())
    {
        image.save(fileName, qPrintable(format));
        screenshotNumber++;
    }
}
void MainWindow::screenshot(QString path)
{
    QImage image = object->grabFrameBuffer();

    QString format = "png";
    QString fileName = path + QString("/screenshot%1.%2").arg(screenshotNumber).arg(format);

    if(!fileName.isEmpty())
    {
        image.save(fileName, qPrintable(format));
        screenshotNumber++;
    }
}

void MainWindow::settingsWindow()
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

        QLabel *rotationSpeed = new QLabel("Speed of the rotation when the buttons Rotate Model \n(X or Y Axis) are clicked.");
        QLabel *tagsRadius = new QLabel("Radius of the spheres created in Distance Mode.");

        QFormLayout *formLayout1 = new QFormLayout;


        formLayout1->addRow("&Ratio Frame/Picture :", framePictureRatioLineEdit);

        formLayout1->addRow("Rotation &Speed (rad/s):", rotationSpeedLineEdit);
        formLayout1->addRow(rotationSpeed);

        formLayout1->addRow("&Tags Radius (m) :", tagsRadiusLineEdit);
        formLayout1->addRow(tagsRadius);


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
        formLayout2->addRow("&Near :", near);
        formLayout2->addRow("&Far :", far);


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

        formLayout3->addRow("&Sensibility (m):", sensibilityLineEdit);
        formLayout3->addRow(sens);

        formLayout3->addRow("&Precision Sensibility (m) :", sensibilityPlusLineEdit);
        formLayout3->addRow(sensPlus);

        sensibility->setLayout(formLayout3);


    tabs->addTab(display, "Display");
    tabs->addTab(camera, "Camera");
    tabs->addTab(sensibility, "Sensibility");
    tabs->adjustSize();


    connect(saveQuitButtons, SIGNAL(accepted()), settings, SLOT(accept()));
    connect(saveQuitButtons, SIGNAL(rejected()), settings, SLOT(reject()));

    if(settings->exec() == QDialog::Accepted)
        sendSettings();
}
void MainWindow::sendSettings()
{
  // DISPLAY
    object->setFramePictureRatio(framePictureRatioLineEdit->text().toFloat());
    object->setRotationSpeed(rotationSpeedLineEdit->text().toFloat());
    object->setTagsRadius(tagsRadiusLineEdit->text().toFloat());

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
void MainWindow::scaleSliderState()
{
    object->scaleSliderState(scaleSlider->isSliderDown());
    object->setCameraSettings(7, (GLfloat)scaleSlider->value()/100);
    focusOFF();
}

void MainWindow::updateModelsList()
{
    list->updateModelsList(object->getModelsList());
}

void MainWindow::focusOFF()     // Disables sliders focus
{
    object->setFocus();
}
