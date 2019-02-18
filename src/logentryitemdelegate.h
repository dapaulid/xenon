#ifndef LOGENTRYITEMDELEGATE_H
#define LOGENTRYITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "highlighter.h"

class LogEntryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit LogEntryItemDelegate(QObject *parent, Highlighters* apHighlighters);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    protected:
        Highlighters* m_pHighlighters;
};

#endif // LOGENTRYITEMDELEGATE_H
