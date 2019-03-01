#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QVariant>
#include <QCache>
#include <QTimer>

#include <vector>

#include "logfileparser.h"
#include "timingstat.h"

const size_t DEFAULT_LINES_PER_CHUNK = 1000;

const size_t NUM_LINES_TO_ANALYZE = 16;

//! maximum line length in characters supported in log files
//! be careful when increasing it, some buffers are allocated on the stack
const size_t MAX_LINE_LENGTH = 1024;

const size_t MAX_LOADED_CHUNKS_PER_FILE = 1000;

const unsigned long DEFAULT_FILE_POLL_INTERVAL_MS = 100;

typedef std::streamoff offset_t;

struct SLogFileEntry
{
    QString line;
    std::vector<QVariant> columns; // use QVector instead of std::vector, as we may make use of copy-on-write
};

struct SLogFileChunk
{
    std::vector<SLogFileEntry> entries;
};

struct SLogFileChunkHeader
{
    offset_t offset;
    size_t bytes;
    size_t lines;
};

class CLogFile: public QObject
{
Q_OBJECT

public:
    CLogFile(const QString& asFileName, QObject* parent = nullptr);
    virtual ~CLogFile();

    QString getLine(size_t index) const;
    QVariant getItem(size_t index, size_t column) const;

    size_t getEntryCount() const {
        return m_uTotalLines;
    }

    size_t getColumnCount() const;
    QString getColumnName(size_t index) const;

signals:
    void changed(size_t oldLineCount, size_t newLineCount);

protected:
    void load();
    void analyze_generic();
    void analyze_ascii();
    void analyze_utf8();
    void clear();

protected slots:
    virtual void timer();

    virtual SLogFileChunk* accessChunk(size_t index) const;
    virtual SLogFileChunk* loadChunk(size_t index) const;

    virtual SLogFileEntry* getEntry(size_t index) const;

    virtual bool updateLastModified();

protected:
    void appendChunkHeader(SLogFileChunkHeader& ch, offset_t offset);

protected:
    QString m_sFileName;
    size_t m_uTotalLines;
    size_t m_uLinesPerChunk;
    std::vector<SLogFileChunkHeader> m_Index;
    mutable QCache<size_t, SLogFileChunk> m_Chunks;
    CLogFileParser* m_pParser;
    mutable CTimingStat m_CacheStat;
    QTimer m_Timer;
    QDateTime m_LastModified;
};


#endif // LOGFILE_H
