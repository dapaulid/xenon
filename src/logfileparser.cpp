#include "logfileparser.h"

#include <QDateTime>

CLogFileParser::CLogFileParser():
    m_eState(eUndecided),
    m_Columns(),
    m_TimestampParser()
{
}

CLogFileParser::~CLogFileParser()
{
}

CLogFileParser::EState CLogFileParser::prepare(const QString& str)
{
    if (m_eState != EState::eUndecided) {
        return m_eState;
    }

    QStringList splitted = str.split("\t");
    if (splitted.empty()) {
        m_eState = EState::eIncapable;
        return m_eState;
    }

    m_Columns.resize(static_cast<size_t>(splitted.size()));
    for (size_t i = 0; i < m_Columns.size(); i++) {
        const QString& s = splitted[static_cast<int>(i)];
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

    m_eState = EState::eReady;
    return m_eState;
}

void CLogFileParser::parseEntry(SLogFileEntry& entry)
{
    QStringList splitted = entry.line.split("\t");
    entry.columns.resize(splitted.size());
    for (auto i = 0; i < splitted.size(); i++) {
        const QString& s = splitted[i];
        switch (m_Columns[static_cast<size_t>(i)].type) {
        case eTimestamp:
            entry.columns[i] = m_TimestampParser.parse(s);
            break;
        default:
            // as string
            entry.columns[i] = splitted[i];
        }
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

