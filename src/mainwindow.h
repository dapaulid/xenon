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
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
    void model_rowsInserted(const QModelIndex & parent, int start, int end);
    void model_modelReset();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void scrollToBottom();

private:
    Ui::CMainWindow *ui;
};

#endif // MAINWINDOW_H
