#include "growingfilemodel.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <QApplication>

GrowingFileModel::GrowingFileModel(QObject *parent, QString filename):
    QAbstractTableModel(parent),
    m_sFilename(filename),
    m_Entries(),
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
    qint64 uFileSize = info.size();
    if (uFileSize < m_uLastPos) {
        clear();
    }
    if (uFileSize > m_uLastPos) {
        update();
    }
}

void GrowingFileModel::clear()
{
    beginResetModel();
    m_Entries.clear();
    m_uLastPos = 0;
    endResetModel();
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

    int lastSize = m_Entries.size();

    QTextStream in(&file);
    in.seek(m_uLastPos);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
        Entry e;
        e.m_sText = line;
        e.m_ts = QDateTime::currentDateTime();
        m_Entries.append(e);
    }
    m_uLastPos = in.pos();

    qDebug() << lastSize << m_Entries.size() << m_uLastPos;
    if (m_Entries.size() > lastSize) {
        beginInsertRows(QModelIndex(), lastSize, m_Entries.size()-1);
        endInsertRows();
    }

    //this->endResetModel();
}

int GrowingFileModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_Entries.size();
}

int GrowingFileModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant GrowingFileModel::data(const QModelIndex &index, int role) const
{
    const Entry& e = m_Entries[index.row()];
    switch (role) {
        case Qt::DisplayRole: {
            switch (index.column()) {
                case 0: return e.m_ts;
                case 1: return e.m_sText;
            }
            break;
        }
        case Qt::BackgroundRole: {
            //if (e.m_bAlternate) {
            if (index.column() == 0) {
                return QPalette().brush(QPalette::AlternateBase);
            }
            break;
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
