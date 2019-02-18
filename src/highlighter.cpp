#include "highlighter.h"

Highlighter::Highlighter(const QString& aName):
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

void Highlighters::Add(Highlighter* apHighlighter)
{
    m_Entries.insert(apHighlighter->GetName(), apHighlighter);
}

Highlighter* Highlighters::Match(const QString& str)
{
    foreach (Highlighter* h, m_Entries) {
        if (h->Matches(str)) {
            return h;
        }
    }
    return nullptr;
}
