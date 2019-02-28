#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QVariant>
#include <QCache>

#include <vector>

#include "logfileparser.h"

const size_t DEFAULT_LINES_PER_CHUNK = 1000;

const size_t NUM_LINES_TO_ANALYZE = 16;

//! maximum line length in characters supported in log files
//! be careful when increasing it, some buffers are allocated on the stack
const size_t MAX_LINE_LENGTH = 1024;

const size_t MAX_LOADED_CHUNKS_PER_FILE = 1000;

typedef std::streamoff offset_t;

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

class CLogFile
{

public:
    CLogFile(const QString& asFileName);
    virtual ~CLogFile();

    SLogFileEntry* getEntry(size_t index) const;

    size_t getEntryCount() const {
        return m_uTotalLines;
    }

    size_t getColumnCount() const;
    QString getColumnName(size_t index) const;

protected:
    virtual void analyze();
    virtual void analyze_generic();
    virtual void analyze_ascii();
    virtual void analyze_utf8();

    virtual SLogFileChunk* accessChunk(size_t index) const;
    virtual SLogFileChunk* loadChunk(size_t index) const;

protected:
    void appendChunkHeader(SLogFileChunkHeader& ch, offset_t offset);

protected:
    QString m_sFileName;
    size_t m_uTotalLines;
    size_t m_uLinesPerChunk;
    std::vector<SLogFileChunkHeader> m_Index;
    mutable QCache<size_t, SLogFileChunk> m_Chunks;
    CLogFileParser* m_pParser;
};


#endif // LOGFILE_H
