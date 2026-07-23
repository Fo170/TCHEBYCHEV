#include <QApplication>
#include <QIcon>
#include "mainwindow.h"
#include "AppConfig.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);

    QIcon icone;
    icone.addFile(":/ico/app-256.png", QSize(256, 256));
    icone.addFile(":/ico/app-128.png", QSize(128, 128));
    icone.addFile(":/ico/app-64.png", QSize(64, 64));
    icone.addFile(":/ico/app-32.png", QSize(32, 32));
    app.setWindowIcon(icone);

    MainWindow w;
    w.setWindowIcon(icone);
    w.show();

    return app.exec();
}