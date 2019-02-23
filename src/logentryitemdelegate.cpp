#include "logentryitemdelegate.h"

#include <QPainter>
#include <QTextDocument>
#include <QTableView>
#include <QDateTime>

CLogEntryItemDelegate::CLogEntryItemDelegate(QTableView* apTableView, Highlighters* apHighlighters):
    QStyledItemDelegate(apTableView),
    m_pTableView(apTableView),
    m_pHighlighters(apHighlighters),
    m_LinePen()
{
    // get color for grid line pen
    // TODO is there a less ugly way?
    QStyleOptionViewItem opt;
    int gridHint = m_pTableView->style()->styleHint(QStyle::SH_Table_GridLineColor, &opt);
    QColor gridColor = static_cast<QRgb>(gridHint);
    m_LinePen = QPen(gridColor, 0, m_pTableView->gridStyle());
}


void CLogEntryItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
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
    initStyleOption(&opt, index);

    // some shorthands
    QVariant data = index.data();
    const QAbstractItemModel* pModel = index.model();
    int row = index.row();

    // do we need to render a timestamp?
    if (data.userType() == QMetaType::QDateTime) {
        // yes -> override text with formatted timestamp
        opt.text = data.toDateTime().toString("hh:mm:ss.zzz");
    }

    // highlight according to text
    QString str = pModel->index(row, 1).data().toString();
    CHighlighter* pHighlighter = m_pHighlighters->Match(str);
    if (pHighlighter) {
        opt.font.setBold(true);
        if (pHighlighter->GetTextColor().isValid()) {
            opt.palette.setColor(QPalette::Text, pHighlighter->GetTextColor());
        }
        if (pHighlighter->GetBackColor().isValid()) {
            painter->fillRect(option.rect, pHighlighter->GetBackColor());
        }
    }

    // draw control explicitly rather than calling base to handle
    // overwritten text in opt.text
    //QStyledItemDelegate::paint(painter, opt, index);
    opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    // get timestamp associated with this row
    QDateTime ts = pModel->index(row, 0).data().toDateTime();
    // get timestamp associated with next row
    QDateTime nts = pModel->index(row+1, 0).data().toDateTime();

    // draw horizontal line to mark time gaps
    if (ts.isValid() && nts.isValid() && ts.msecsTo(nts) > 500) {
        QPen oldPen = painter->pen();
        painter->setPen(m_LinePen);
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        painter->setPen(oldPen);
    }
}

QSize CLogEntryItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
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
