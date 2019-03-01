#ifndef LOGFILEWIDGET_H
#define LOGFILEWIDGET_H

#include <QWidget>

#include "logfile.h"

namespace Ui {
class CLogFileWidget;
}

class CLogFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CLogFileWidget(QWidget *parent, const QString& fileName);
    ~CLogFileWidget();

    QString getDisplayName() const {
        return m_LogFile.getDisplayName();
    }

private slots:
    void model_rowsInserted(const QModelIndex & parent, int start, int end);
    void scrollToBottom();

private:
    Ui::CLogFileWidget *ui;
    CLogFile m_LogFile;
};

#endif // LOGFILEWIDGET_H