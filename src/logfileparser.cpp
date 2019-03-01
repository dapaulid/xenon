#include <QRegularExpression>
#include <QDateTime>
#include <QDebug>

#include "logfileparser.h"
#include "histogram.h"


CLogFileParser::CLogFileParser():
    m_Columns(),
    m_TimestampParser()
{
}

CLogFileParser::~CLogFileParser()
{
}

size_t getColumnIndex(const QString& str, int index)
{
    if (index > str.length()) {
        index = str.length();
    }

    size_t column = 0;
    int i = 0;
    while (i < index) {
        // consume leading whitespace
        while (i < index && str[i].isSpace()) {
            i++;
        }
        // consume non-whitespace
        // TODO handle escaping
        while (i < index && !str[i].isSpace()) {
            i++;
        }
        if (i < index) {
            column++;
        }
    }
    return column;
}

QString getColumn(const QString& str, size_t column, size_t columnCount)
{
    bool bIsLastColumn = column == columnCount-1;
    int i = 0;
    while (i < str.length()) {
        // consume leading whitespace
        while (i < str.length() && str[i].isSpace()) {
            i++;
        }
        // consume non-whitespace
        // TODO handle escaping
        int startPos = i;
        while (i < str.length() && !str[i].isSpace()) {
            i++;
        }
        if (column == 0) {
            if (bIsLastColumn) {
                return str.mid(startPos);
            } else {
                return str.mid(startPos, i - startPos);
            }
        } else {
            --column;
        }

    }
    return QString();
}

void CLogFileParser::prepare(const std::vector<QString>& lines)
{
    CHistogram<size_t> hist;
    QRegularExpression re("\\b(\\p{L}{2,})\\s+\\w{2,}");

    for (size_t i = 0; i < lines.size(); i++) {
        const QString& line = lines[i];
        size_t uColumnCount = 0;
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            qDebug() <<  match.captured(1);
            uColumnCount = getColumnIndex(line, match.capturedStart(1))+1;
            qDebug("Line %lu: Columns = %lu", i+1, uColumnCount);
        } else {
            uColumnCount = 1;
            qDebug("Line %lu: Columns = %lu (no match)", i+1, uColumnCount);
        }
        hist.insert(uColumnCount);
    }

    // TODO generalize
    const QString& line = lines[0];


    m_Columns.resize(hist.mode());
    for (size_t i = 0; i < m_Columns.size(); i++) {
        QString s = getColumn(line, i, m_Columns.size());
        SColumnInfo& col = m_Columns[i];

        QDateTime ts = m_TimestampParser.parse(s);
        if (ts.isValid()) {
            // as timestamp
            col.name = "Timestamp";
            col.type = ELogFileColumnType::eTimestamp;
        } else {
            // as string
            col.name = "Text";
            col.type = ELogFileColumnType::eText;
        }
    }

}

QVariant CLogFileParser::parseLine(const QString& line, size_t column)
{
    QString item = getColumn(line, column, m_Columns.size());
    switch (m_Columns[column].type) {
    case eTimestamp:
        return m_TimestampParser.parse(item);
    default:
        // as string
        return item;
    }
}

size_t CLogFileParser::getColumnCount() const
{
    return m_Columns.size();
}

QString CLogFileParser::getColumnName(size_t index) const
{
    return m_Columns[index].name;
}

