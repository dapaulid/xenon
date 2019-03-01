#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

public slots:
    void open(const QString& filename);

private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::CMainWindow *ui;
};

#endif // MAINWINDOW_H
