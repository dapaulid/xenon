#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logfilewidget.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    // TODO how to remove tabs in designer?
    ui->tabWidget->clear();
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::open(const QString& filename)
{
    // create new log file widget
    CLogFileWidget* logFileWidget = new CLogFileWidget(this, filename);
    // create new tab for it
    int tabIndex = ui->tabWidget->addTab(logFileWidget, logFileWidget->getDisplayName());
    // bring it to front
    ui->tabWidget->setCurrentIndex(tabIndex);
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
