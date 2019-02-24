#include "logfileparser.h"

#include <QDateTime>

CLogFileParser::CLogFileParser():
    m_TimestampParser()
{
}

CLogFileParser::~CLogFileParser()
{
}

void CLogFileParser::parseEntry(SLogFileEntry& entry)
{
    QStringList splitted = entry.line.split("\t");
    entry.columns.resize(splitted.size());
    for (auto i = 0; i < splitted.size(); i++) {
        const QString& s = splitted[i];
        QDateTime ts = m_TimestampParser.parse(s);
        if (ts.isValid()) {
            // as timestamp
            entry.columns[i] = ts;
        } else {
            // as string
            entry.columns[i] = splitted[i];
        }
    }
}

size_t CLogFileParser::getColumnCount() const
{
    return 2;
}

QString CLogFileParser::getColumnName(size_t index) const
{
    return QString("#%0").arg(index);
}

