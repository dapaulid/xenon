#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QVector>

const quint32 DEFAULT_LINES_PER_CHUNK = 1000;

//! maximum line length in characters supported in log files
//! be careful when increasing it, some buffers are allocated on the stack
const quint32 MAX_LINE_LENGTH = 1024;

class LogFile
{
public:
    LogFile(const QString& asFileName);
    virtual ~LogFile();

protected:
    virtual void analyze();
    virtual void analyze_generic();
    virtual void analyze_ascii();
    virtual void analyze_utf8();

protected:
    struct ChunkHeader {
        qint64 offset;
        quint32 bytes;
        quint32 lines;
    };

    void appendChunkHeader(ChunkHeader& ch, qint64 offset);

protected:
    QString m_sFileName;
    quint64 m_uTotalLines;
    quint32 m_uLinesPerChunk;
    QVector<ChunkHeader> m_Index;
};

#endif // LOGFILE_H
