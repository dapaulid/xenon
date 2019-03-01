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
    CLogFileWidget* logFileWidget = new CLogFileWidget(ui->tabWidget, filename);
    ui->tabWidget->addTab(logFileWidget, logFileWidget->getDisplayName());
}

void CMainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
