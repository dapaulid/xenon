#ifndef LOGFILEMODEL_H
#define LOGFILEMODEL_H


#include <QAbstractTableModel>
#include <QStringList>
#include <QTimer>
#include <QDateTime>

#include "logfile.h"

class CLogFileModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CLogFileModel(QObject *parent, const CLogFile& logfile);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected slots:
    void logFileChanged(size_t oldLineCount, size_t newLineCount);

protected:
    const CLogFile& m_LogFile;
};

#endif // LOGFILEMODEL_H
