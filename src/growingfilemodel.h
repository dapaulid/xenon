#ifndef GROWINGFILEMODEL_H
#define GROWINGFILEMODEL_H


#include <QAbstractTableModel>
#include <QStringList>
#include <QTimer>
#include <QDateTime>

class CGrowingFileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    struct Entry {
        QDateTime m_ts;
        QString m_sText;
    };

public:
    CGrowingFileModel(QObject *parent, QString filename);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected slots:
    void update();
    void timer();
    void clear();


protected:
    QString m_sFilename;
    QList<Entry> m_Entries;
    QTimer m_Timer;
    qint64 m_uLastPos;
};

#endif // GROWINGFILEMODEL_H
