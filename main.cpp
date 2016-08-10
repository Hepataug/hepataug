#include "mainWindow.h"



int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow fenetre;
    fenetre.setWindowTitle(QString::fromUtf8("HEPATAUG"));
    fenetre.show();

    return app.exec();
}
