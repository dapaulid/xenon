#include <QFileDialog>
#include <QCloseEvent>
#include <QDrag>
#include <QMimeData>
#include <QSettings>
#include <QVariant>
#include <QDebug>


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logfilewidget.h"
#include "aboutdialog.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    // TODO how to remove tabs in designer?
    ui->tabWidget->clear();

    readSettings();
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::open(const QString& filename)
{
    QFileInfo fi(filename);

    // check for existence
    if (!fi.exists()) {
        qWarning() << "Cannot open" << filename << ": File does not exist";
        return;
    }

    // check if already open
    for (int i = 0; i < getTabCount(); i++) {
        const CLogFileWidget* tab = getTab(i);
        if (tab && (fi == QFileInfo(tab->getFileName()))) {
            // found -> bring it to front
            ui->tabWidget->setCurrentIndex(i);
            // we're done
            return;
        }
    }

    // create new log file widget
    CLogFileWidget* lfw = new CLogFileWidget(this, filename);
    // create new tab for it
    int tabIndex = ui->tabWidget->addTab(lfw, lfw->getDisplayName());
    // set filename as tooltip
    ui->tabWidget->setTabToolTip(tabIndex, lfw->getFileName());
    // bring it to front
    ui->tabWidget->setCurrentIndex(tabIndex);

    ui->statusBar->showMessage(QString("Opened %1").arg(filename), 5000);
}

void CMainWindow::readSettings()
{
    qInfo("loading application settings");
    QSettings settings;

    restoreGeometry(settings.value("geometry").toByteArray());

    // restore open tabs
    QStringList openTabs = settings.value("openTabs").toStringList();
    foreach (QString tab, openTabs) {
        open(tab);
    }
}

void CMainWindow::writeSettings() const
{
    qInfo("saving application settings");
    QSettings settings;

    settings.setValue("geometry", saveGeometry());

    // persist open tabs
    QStringList openTabs;
    for (int i = 0; i < getTabCount(); i++) {
        const CLogFileWidget* lfw = getTab(i);
        if (lfw) {
            openTabs.append(lfw->getFileName());
        }
    }
    settings.setValue("openTabs", openTabs);
}

void CMainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void CMainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        event->acceptProposedAction();
    }
}

void CMainWindow::dropEvent(QDropEvent *event)
{
    // check for our needed mime type, here a file or a list of files
    if (event->mimeData()->hasUrls()) {
        QStringList pathList;
        QList<QUrl> urlList = event->mimeData()->urls();

        // open by local paths of the files
        for (int i = 0; i < urlList.size(); i++) {
            open(urlList.at(i).toLocalFile());
        }
    }

    event->acceptProposedAction();
}

int CMainWindow::getTabCount() const
{
    return ui->tabWidget->count();
}

const CLogFileWidget* CMainWindow::getTab(int index) const
{
    return dynamic_cast<const CLogFileWidget*>(ui->tabWidget->widget(index));
}

void CMainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void CMainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Log File"), QString(), tr("Log files (*.log *.txt);;All files (*)"));

    if (!fileName.isEmpty()) {
        open(fileName);
    }
}

#include <QMessageBox>

void CMainWindow::on_actionAbout_triggered()
{
    qDebug() << "Foobar";
    CAboutDialog about(this);
    about.exec();
}
