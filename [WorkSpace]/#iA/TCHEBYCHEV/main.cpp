#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("TCHEBYCHEV");
    app.setApplicationVersion("2.0");

    MainWindow w;
    w.show();

    return app.exec();
}
