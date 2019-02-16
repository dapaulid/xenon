#include "growingfilemodel.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

GrowingFileModel::GrowingFileModel(QObject *parent, QString filename):
    QAbstractTableModel(parent),
    m_sFilename(filename),
    m_Lines(),
    m_Timer(),
    m_uLastPos()
{
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(timer()));
    update();
    m_Timer.start(100);
}

void GrowingFileModel::timer()
{
    /*
     * We use a timer rather than QFileSystemWatcher, because
     * when I checked last (Qt 5.7 under Windows) it was very
     * buggy and catched only modifications after close()ing a file,
     * and not even all of them...
     */
    QFileInfo info(m_sFilename);
    if (info.size() > m_uLastPos) {
        update();
    }
}

void GrowingFileModel::update()
{
    qDebug() << "UPDATE";
    //this->beginResetModel();

    //m_Lines.clear();

    QFile file(m_sFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "fuck";
        return;
    }

    int lastSize = m_Lines.size();

    QTextStream in(&file);
    in.seek(m_uLastPos);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
        m_Lines.append(line);
    }
    m_uLastPos = in.pos();

    qDebug() << lastSize << m_Lines.size() << m_uLastPos;
    if (m_Lines.size() > lastSize) {
        beginInsertRows(QModelIndex(), lastSize, m_Lines.size()-1);
        endInsertRows();
    }

    //this->endResetModel();
}

int GrowingFileModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_Lines.size();
}

int GrowingFileModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant GrowingFileModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return "???";
            case 1: return m_Lines[index.row()];
        }
    }
    return QVariant();
}

QVariant GrowingFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return tr("Timestamp");
                case 1: return tr("Text");
            }
        } else if (orientation == Qt::Vertical) {
            return section+1;
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            return Qt::AlignLeft;
        } else if (orientation == Qt::Vertical) {
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }
    }
    return QVariant();
}

Qt::ItemFlags GrowingFileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
