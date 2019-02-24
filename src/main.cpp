#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "logfile.h"

int main(int argc, char *argv[])
{
    if (argc > 2 && strcmp(argv[1], "-i") == 0) {
        CLogFile lf(argv[2]);
        return 0;
    }

    qSetMessagePattern("%{time yyyy-MM-ddTHH:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif} %{file}:%{line} - %{message}");

    QApplication a(argc, argv);
    CMainWindow w;
    w.show();

    return a.exec();
}
