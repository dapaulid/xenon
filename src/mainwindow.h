#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logfilewidget.h"

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = nullptr);
    virtual ~CMainWindow() override;

    int getTabCount() const;
    const CLogFileWidget* getTab(int index) const;

public slots:
    void open(const QString& filename);

protected slots:
    void readSettings();
    void writeSettings() const;

// overrides
protected:
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

private slots:
    void on_tabWidget_tabCloseRequested(int index);

    void on_action_Open_triggered();

    void on_actionAbout_triggered();

private:
    Ui::CMainWindow *ui;
};

#endif // MAINWINDOW_H
