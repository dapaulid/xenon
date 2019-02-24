#ifndef LOGFILEPARSER_H
#define LOGFILEPARSER_H

#include <vector>

#include <QString>
#include <QVariant>
#include <QVector>

#include "timestampparser.h"

struct SLogFileEntry
{
    QString line;
    QVector<QVariant> columns; // use QVector instead of std::vector, as we may make use of copy-on-write
};

enum ELogFileColumnType {
    eText,
    eTimestamp
};

class CLogFileParser
{
public:
    enum EState {
        eUndecided,
        eReady,
        eIncapable
    };

    CLogFileParser();
    virtual ~CLogFileParser();

    virtual EState prepare(const QString& str);
    virtual void parseEntry(SLogFileEntry& entry);

    virtual size_t getColumnCount() const;
    virtual QString getColumnName(size_t index) const;

    EState getState() const {
        return m_eState;
    }

protected:
    struct SColumnInfo {
        QString name;
        ELogFileColumnType type;
    };

protected:
    EState m_eState;
    std::vector<SColumnInfo> m_Columns;
    CTimestampParser m_TimestampParser;
};

#endif // LOGFILEPARSER_H
