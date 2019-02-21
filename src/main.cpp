#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "logfile.h"

int main(int argc, char *argv[])
{
    if (argc > 2 && strcmp(argv[1], "-i") == 0) {
        LogFile lf(argv[2]);
        return 0;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
