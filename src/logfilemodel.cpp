#include "logfilemodel.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <QApplication>

CLogFileModel::CLogFileModel(QObject *parent, QString filename):
    QAbstractTableModel(parent),
    m_sFilename(filename),
    m_LogFile(filename)
{
}

int CLogFileModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_LogFile.getEntryCount();
}

int CLogFileModel::columnCount(const QModelIndex & /*parent*/) const
{
    return m_LogFile.getColumnCount();
}

QVariant CLogFileModel::data(const QModelIndex &index, int role) const
{
    // TODO fix const cast
    const SLogFileEntry* pEntry = m_LogFile.getEntry(index.row());
    if (!pEntry) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole: {
            return pEntry->columns[index.column()];
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

QVariant CLogFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return m_LogFile.getColumnName(static_cast<size_t>(section));
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

Qt::ItemFlags CLogFileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
