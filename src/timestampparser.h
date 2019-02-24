#ifndef TIMESTAMPPARSER_H
#define TIMESTAMPPARSER_H

#include <QString>
#include <QDateTime>

class CTimestampParser
{
public:
    CTimestampParser();

    QDateTime parse(const QString& str);

private:
    size_t m_uDateFormat;
};

#endif // TIMESTAMPPARSER_H
