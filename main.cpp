#include "mainWindow.h"



int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow fenetre;                                     // Creation of an object MainWindow
    fenetre.setWindowTitle(QString::fromUtf8("HEPATAUG"));  // Window title
    fenetre.show();

    return app.exec();
}
