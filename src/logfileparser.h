#ifndef LOGFILEPARSER_H
#define LOGFILEPARSER_H

#include <vector>

#include <QString>
#include <QVariant>

#include "timestampparser.h"

enum ELogFileColumnType {
    eText,
    eTimestamp
};

class CLogFileParser
{
public:
    CLogFileParser();
    virtual ~CLogFileParser();

    virtual void prepare(const std::vector<QString>& lines);
    virtual QVariant parseLine(const QString& line, size_t column);

    virtual size_t getColumnCount() const;
    virtual QString getColumnName(size_t index) const;

protected:
    struct SColumnInfo {
        QString name;
        ELogFileColumnType type;
    };

protected:
    std::vector<SColumnInfo> m_Columns;
    CTimestampParser m_TimestampParser;
};

#endif // LOGFILEPARSER_H
