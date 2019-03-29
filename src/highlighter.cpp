#include <QRegularExpressionMatchIterator>
#include <QTextStream>
#include <QStack>

#include "highlighter.h"

CHighlighter::CHighlighter(const QString& aName, bool abFullLine):
    m_Name(aName),
    m_bFullLine(abFullLine),
    m_RegExp(),
    m_TextColor(),
    m_BackColor()
{
}

bool CHighlighter::highlight(QString& str)
{
    if (!m_RegExp.isValid() || m_RegExp.pattern().isEmpty()) {
        return false;
    }
    bool bHighlighted = false;
    QRegularExpressionMatchIterator it = m_RegExp.globalMatch(str);
    // use stack to reverse order of match processing
    // this is required to avoid invalidating strnig index
    QStack<QRegularExpressionMatch> matches;
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        if (match.hasMatch()) {
            matches.push(match);
        }
    }
    while (!matches.empty()) {
        QRegularExpressionMatch match = matches.pop();
        // insert end tag first to avoid invalidating index
        str.insert(match.capturedEnd(), QString("</%1>").arg(m_Name));
        str.insert(match.capturedStart(), QString("<%1>").arg(m_Name));
        bHighlighted = true;
    }
    return bHighlighted;
}

Highlighters::Highlighters():
    m_Entries()
{
}

void Highlighters::Add(CHighlighter* apHighlighter)
{
    m_Entries.insert(apHighlighter->GetName(), apHighlighter);
}

CHighlighter* Highlighters::Match(const QString& str)
{
    foreach (CHighlighter* h, m_Entries) {
        if (h->isFullLine() && h->Matches(str)) {
            return h;
        }
    }
    return nullptr;
}

QString Highlighters::styleSheet() const
{
    QString styleSheet;
    QTextStream ss(&styleSheet);
    // iterate over items
    foreach (CHighlighter* h, m_Entries) {
        // begin entry
        ss << h->GetName() << " { ";
        // text color
        if (h->GetTextColor().isValid()) {
            ss << "color: " << h->GetTextColor().name() << "; ";
        }
        // background color
        if (h->GetBackColor().isValid()) {
            ss << "background-color: " << h->GetBackColor().name() << "; ";
        }
        // end entry
        ss << "}" << endl;
    }

    return styleSheet;
}
