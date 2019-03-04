#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class CAboutDialog;
}

class CAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAboutDialog(QWidget *parent = nullptr);
    ~CAboutDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::CAboutDialog *ui;
};

#endif // ABOUTDIALOG_H
