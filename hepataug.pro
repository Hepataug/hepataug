QT += opengl

LIBS += -lGLU -L/usr/local/lib -lopencv_core -lopencv_videoio -lopencv_imgproc

SOURCES += \
    main.cpp \
    mainWindow.cpp \
    openGLWidget.cpp \
    trackBall.cpp \
    GLmodel.cpp \
    GLtexture.cpp \
    modelsListWidget.cpp

HEADERS += \
    mainWindow.h \
    openGLWidget.h \
    trackBall.h \
    GLmodel.h \
    GLtexture.h \
    modelsListWidget.h
