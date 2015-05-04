#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    talga::editor::MainWindow w;
    w.show();

    qDebug() << "HELLO!!!";

    return a.exec();
}
