#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void model_rowsInserted(const QModelIndex & parent, int start, int end);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void scrollToBottom();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
