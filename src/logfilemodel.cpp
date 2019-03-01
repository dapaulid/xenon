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
    connect(&m_LogFile, SIGNAL(grown(size_t, size_t)), this, SLOT(logFileGrown(size_t, size_t)));
}

void CLogFileModel::logFileGrown(size_t oldLineCount, size_t newLineCount)
{
    beginInsertRows(QModelIndex(), static_cast<int>(oldLineCount), static_cast<int>(newLineCount)-1);
    endInsertRows();
}

int CLogFileModel::rowCount(const QModelIndex & /*parent*/) const
{
   return static_cast<int>(m_LogFile.getEntryCount());
}

int CLogFileModel::columnCount(const QModelIndex & /*parent*/) const
{
    return static_cast<int>(m_LogFile.getColumnCount());
}

QVariant CLogFileModel::data(const QModelIndex &index, int role) const
{
    size_t row = static_cast<size_t>(index.row());
    size_t col = static_cast<size_t>(index.column());

    switch (role) {
        case Qt::DisplayRole: {
            return m_LogFile.getItem(row, col);
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
