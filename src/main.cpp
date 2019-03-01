#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

#include "logfile.h"

void displayLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    switch (type) {
    case QtWarningMsg:
        QMessageBox::warning(nullptr, "Warning", msg);
        break;
    case QtCriticalMsg:
        QMessageBox::critical(nullptr, "Critical Error", msg);
        break;
    case QtFatalMsg:
        QMessageBox::critical(nullptr, "Fatal Error", msg);
        break;
    default:
        ;// do nothing
    }
}

int main(int argc, char *argv[])
{
    if (argc > 2 && strcmp(argv[1], "-i") == 0) {
        CLogFile lf(argv[2]);
        return 0;
    }

    qInstallMessageHandler(displayLogMessageHandler);
    qSetMessagePattern("%{time yyyy-MM-ddTHH:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif} %{file}:%{line} - %{message}");

    QApplication a(argc, argv);

    CMainWindow w;
    w.show();

    return a.exec();
}
