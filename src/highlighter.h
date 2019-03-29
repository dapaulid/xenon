#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegularExpression>
#include <QColor>
#include <QMap>

class CHighlighter
{
public:
    CHighlighter(const QString& aName, bool abFullLine);

    const QString& GetName() const {
        return m_Name;
    }

    bool isFullLine() const {
        return m_bFullLine;
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

    bool highlight(QString& str);

protected:
    QString m_Name;
    bool m_bFullLine;
    QRegularExpression m_RegExp;
    QColor m_TextColor;
    QColor m_BackColor;
};

class Highlighters
{
public:
    Highlighters();

    void Add(CHighlighter* apHighlighter);

    CHighlighter* Match(const QString& str);

    QString styleSheet() const;

    CHighlighter* get(const QString& name) {
        return m_Entries[name];
    }

protected:
    QMap<QString, CHighlighter*> m_Entries;
};

#endif // HIGHLIGHTER_H
