#ifndef LOGFILEPARSER_H
#define LOGFILEPARSER_H

#include <QString>
#include <QVariant>
#include <QVector>

struct SLogFileEntry
{
    QString line;
    QVector<QVariant> columns; // use QVector instead of std::vector, as we may make use of copy-on-write
};


class CLogFileParser
{
public:
    CLogFileParser();
    virtual ~CLogFileParser();

    virtual void parseEntry(SLogFileEntry& entry);

    virtual size_t getColumnCount() const;
    virtual QString getColumnName(size_t index) const;
};

#endif // LOGFILEPARSER_H
