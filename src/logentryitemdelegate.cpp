#include "logentryitemdelegate.h"

#include <QPainter>
#include <QTextDocument>
#include <QTableView>

LogEntryItemDelegate::LogEntryItemDelegate(QTableView* apTableView, Highlighters* apHighlighters):
    QStyledItemDelegate(apTableView),
    m_pTableView(apTableView),
    m_pHighlighters(apHighlighters),
    m_LinePen()
{
    int gridHint = m_pTableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    m_LinePen = QPen(gridColor, 0, m_pTableView->gridStyle());
}


void LogEntryItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QVariant data = index.data(); // pick the data you need here
    opt.font.setBold(true);
    opt.backgroundBrush = QBrush(Qt::red);

    painter->setPen(Qt::red);
    painter->setPen( option.palette.highlightedText().color() );

    QStyledItemDelegate::paint(painter, opt, index);
  */

/*
    auto options = option;
    initStyleOption(&options, index);

    painter->save();

    QTextDocument doc;
    doc.setHtml(options.text.toHtmlEscaped());

    options.text = "";
    painter->setBrush(Qt::red);
    painter->setPen(Qt::red);
    options.backgroundBrush = QBrush(Qt::red);
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter);

    painter->translate(options.rect.left(), options.rect.top());
    QRect clip(0, 0, options.rect.width(), options.rect.height());
    doc.drawContents(painter, clip);

    painter->restore();
*/
    QStyleOptionViewItem opt = option;



    //QString str = index.data().toString();
    QString str = index.model()->index(index.row(), 1).data().toString();
    Highlighter* pHighlighter = m_pHighlighters->Match(str);
    if (pHighlighter) {
        opt.font.setBold(true);
        if (pHighlighter->GetTextColor().isValid()) {
            opt.palette.setColor(QPalette::Text, pHighlighter->GetTextColor());
        }
        if (pHighlighter->GetBackColor().isValid()) {
            painter->fillRect(option.rect, pHighlighter->GetBackColor());
        }
    }

    //opt.palette.setColor(QPalette::Text, Qt::red);
    //opt.palette.setBrush(QPalette::Text, Qt::red);
    //opt.font.setBold(true);
    //opt.palette.setColor(QPalette::Normal, QPalette::Background, Qt::blue);
    //opt.palette.setColor(QPalette::Text, Qt::red);
    //opt.palette.setBrush(QPalette::Background, Qt::blue);
    //painter->fillRect(option.rect, Qt::yellow);
    QStyledItemDelegate::paint(painter, opt, index);



    // draw horizontalLine
    QPen oldPen = painter->pen();
    painter->setPen(m_LinePen);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    painter->setPen(oldPen);
}

QSize LogEntryItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
    /*
    QStyleOptionViewItem options = option;
    initStyleOption(&options, index);

    QTextDocument doc;
    doc.setHtml(options.text);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
    */
}
