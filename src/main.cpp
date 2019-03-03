#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

#include "logfile.h"

static QtMessageHandler s_DefaultMsgHandler;

void displayLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // call default handler
    s_DefaultMsgHandler(type, context, msg);

    QString what;
    QMessageBox::Icon icon;

    // determine dialog type
    switch (type) {
    case QtWarningMsg:
        what = "warning";
        icon = QMessageBox::Warning;
        break;
    case QtCriticalMsg:
        what = "critical error";
        icon = QMessageBox::Critical;
        break;
    case QtFatalMsg:
        what = "fatal error";
        icon = QMessageBox::Critical;
        break;
    default:
        // do not display
        return;
    }

    // setup dialog
    QMessageBox dialog;

    dialog.setIcon(icon);
    dialog.setText(QString("A %1 occurred while executing %2:\n\n%3")
        .arg(what).arg(QCoreApplication::applicationName()).arg(msg));
    dialog.setDetailedText(QString("category:\t %1\nlocation:\t %2:%3\nfunction:\t %4")
        .arg(context.category)
        .arg(context.file).arg(context.line)
        .arg(context.function));

    // show it
    dialog.exec();
}

int main(int argc, char *argv[])
{
    // set application info
    QCoreApplication::setApplicationName("Xenon Log File Viewer");
    QCoreApplication::setOrganizationName("dapaulid");
    QCoreApplication::setApplicationVersion(GIT_VERSION);

    if (argc > 2 && strcmp(argv[1], "-i") == 0) {
        CLogFile lf(argv[2]);
        return 0;
    }

    s_DefaultMsgHandler = qInstallMessageHandler(displayLogMessageHandler);
    qSetMessagePattern("%{time yyyy-MM-ddTHH:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif} %{file}:%{line} - %{message}");

    QApplication a(argc, argv);

    // trace version info
    qInfo("Starting %s, version %s",
        QCoreApplication::applicationName().toLatin1().data(),
        QCoreApplication::applicationVersion().toLatin1().data());

    CMainWindow w;
    w.open(QCoreApplication::arguments().at(1));
    w.show();

    QString version(GIT_VERSION);

    return a.exec();
}
