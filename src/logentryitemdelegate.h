#ifndef LOGENTRYITEMDELEGATE_H
#define LOGENTRYITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableView>
#include <QPen>

#include "highlighter.h"

class LogEntryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit LogEntryItemDelegate(QTableView* apTableView, Highlighters* apHighlighters);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    protected:
        QTableView* m_pTableView;
        Highlighters* m_pHighlighters;
        QPen m_LinePen;
};

#endif // LOGENTRYITEMDELEGATE_H
