#include "timestampparser.h"

//! table of all supported Qt date formats
static const Qt::DateFormat  s_DateFormats[] = {
    Qt::DateFormat::TextDate,      // default Qt
    Qt::DateFormat::ISODate,       // ISO 8601
    //Qt::DateFormat::SystemLocaleDate, // deprecated
    //Qt::DateFormat::LocalDate = SystemLocaleDate, // deprecated
    //Qt::DateFormat::LocaleDate,     // deprecated
    Qt::DateFormat::SystemLocaleShortDate,
    Qt::DateFormat::SystemLocaleLongDate,
    Qt::DateFormat::DefaultLocaleShortDate,
    Qt::DateFormat::DefaultLocaleLongDate,
    Qt::DateFormat::RFC2822Date,        // RFC 2822 (+ 850 and 1036 during parsing)
    Qt::DateFormat::ISODateWithMs
};

#define ARRAY_LENGTH(arr) (sizeof((arr))/sizeof((arr)[0]))

CTimestampParser::CTimestampParser():
    m_uDateFormat(0)
{
}

QDateTime CTimestampParser::parse(const QString& str)
{
    size_t uLastDateFormat = m_uDateFormat;
    QDateTime ts = QDateTime::fromString(str, s_DateFormats[m_uDateFormat]);
    while (!ts.isValid()) {
        // try next format
        m_uDateFormat = (m_uDateFormat + 1) % ARRAY_LENGTH(s_DateFormats);
        if (m_uDateFormat == uLastDateFormat) {
            // tried everything, give up
            break;
        }
        ts = QDateTime::fromString(str, s_DateFormats[m_uDateFormat]);
    }
    return ts;
}
