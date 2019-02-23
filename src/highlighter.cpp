#include "highlighter.h"

CHighlighter::CHighlighter(const QString& aName):
    m_Name(aName),
    m_RegExp(),
    m_TextColor(),
    m_BackColor()
{
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
        if (h->Matches(str)) {
            return h;
        }
    }
    return nullptr;
}
