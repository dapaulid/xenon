#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
/*
    QFile file("C:\\Users\\dapau\\projects\\log-analyzer\\test.log");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
    }

    return 0;
   */
}
