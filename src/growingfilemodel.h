#ifndef GROWINGFILEMODEL_H
#define GROWINGFILEMODEL_H


#include <QAbstractTableModel>
#include <QStringList>
#include <QTimer>

class GrowingFileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    GrowingFileModel(QObject *parent, QString filename);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected slots:
    void update();
    void timer();

protected:
    QString m_sFilename;
    QStringList m_Lines;
    QTimer m_Timer;
    qint64 m_uLastPos;
};

#endif // GROWINGFILEMODEL_H
