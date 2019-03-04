#include "aboutdialog.h"
#include "ui_aboutdialog.h"

CAboutDialog::CAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutDialog)
{
    ui->setupUi(this);

    ui->lblApp->setText(QCoreApplication::applicationName());
    ui->lblQt->setText(ui->lblQt->text() + QT_VERSION_STR);

    ui->lblInstalled->setText(GIT_VERSION);

}

CAboutDialog::~CAboutDialog()
{
    delete ui;
}

void CAboutDialog::on_btnClose_clicked()
{
    close();
}
