#include <QApplication>
#include <QtGui>
#include "mainwidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWidget x;
    x.show();

    return app.exec();
}

