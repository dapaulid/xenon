#ifndef LOGENTRYITEMDELEGATE_H
#define LOGENTRYITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableView>
#include <QPen>
#include <QTextDocument>

#include "highlighter.h"

class CLogEntryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit CLogEntryItemDelegate(QTableView* apTableView, Highlighters* apHighlighters);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    protected:
        QTableView* m_pTableView;
        Highlighters* m_pHighlighters;
        CHighlighter* m_pMarkHiLi;
        QPen m_LinePen;
        mutable QTextDocument m_Doc;
};

#endif // LOGENTRYITEMDELEGATE_H
