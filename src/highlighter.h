#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegExp>
#include <QColor>
#include <QMap>

class Highlighter
{
public:
    Highlighter(const QString& aName);

    const QString& GetName() const {
        return m_Name;
    }

    void SetPattern(const QString& aPattern) {
        m_RegExp.setPattern(aPattern);
    }

    void SetTextColor(const QColor& aColor) {
        m_TextColor = aColor;
    }

    const QColor& GetTextColor() const {
        return m_TextColor;
    }

    void SetBackColor(const QColor& aColor) {
        m_BackColor = aColor;
    }

    const QColor& GetBackColor() const {
        return m_BackColor;
    }

    bool Matches(const QString& str) {
        return str.contains(m_RegExp);
    }

protected:
    QString m_Name;
    QRegExp m_RegExp;
    QColor m_TextColor;
    QColor m_BackColor;
};

class Highlighters
{
public:
    Highlighters();

    void Add(Highlighter* apHighlighter);

    Highlighter* Match(const QString& str);

protected:
    QMap<QString, Highlighter*> m_Entries;
};

#endif // HIGHLIGHTER_H
