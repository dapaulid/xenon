#include "logfile.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

#include <fstream>
#include <locale>
#include <codecvt>

LogFile::LogFile(const QString& asFileName):
    m_sFileName(asFileName),
    m_uTotalLines(0),
    m_uLinesPerChunk(DEFAULT_LINES_PER_CHUNK),
    m_Index()
{
    analyze();
}

LogFile::~LogFile()
{
}

void LogFile::analyze()
{
    //analyze_utf8();
    analyze_ascii();   // fastest, but there may be issues on utf-8 files (false line breaks?)
    //analyze_generic(); // additionally finds codec, but broken QTextStream::pos()

    qDebug() << "Lines:" << m_uTotalLines;
    qDebug() << "Chunks:" << m_Index.count();
}

void LogFile::analyze_ascii()
{
    std::ifstream in(m_sFileName.toStdString());

    if (!in.good()) {
        qDebug() << "Failed to open " << m_sFileName;
        return;
    }

    ChunkHeader ch = {};
    ch.offset = in.tellg();

    char line [MAX_LINE_LENGTH];
    while (in.getline(line, sizeof(line))) {
        m_uTotalLines++;
        ch.lines++;
        if (ch.lines == m_uLinesPerChunk) {
            appendChunkHeader(ch, in.tellg());
        }
    }

    in.clear(); // reset error flags for final tellg to work
    appendChunkHeader(ch, in.tellg());
}

void LogFile::analyze_utf8()
{
    std::locale old_locale;
    std::locale utf8_locale(old_locale, new std::codecvt_utf8<wchar_t,0x10ffff, std::consume_header>);

    std::wifstream in(m_sFileName.toStdString());
    in.imbue(std::locale(utf8_locale));

    if (!in.good()) {
        qDebug() << "Failed to open " << m_sFileName;
        return;
    }

    ChunkHeader ch = {};
    ch.offset = in.tellg();

    wchar_t line [MAX_LINE_LENGTH];
    while (in.getline(line, sizeof(line))) {
        m_uTotalLines++;
        ch.lines++;
        if (ch.lines == m_uLinesPerChunk) {
            appendChunkHeader(ch, in.tellg());
        }
    }

    in.clear(); // reset error flags for final tellg to work
    appendChunkHeader(ch, in.tellg());
}

void LogFile::analyze_generic()
{
    QFile file(m_sFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "fuck";
        return;
    }

    QTextStream in(&file);

    /*
     * TODO: pure readLineInto() is 1.5 times faster than readLine(),
     * but still takes about 1024 ms for a 0.5 GB file
     * on my i7 8th gen notebook with SSD. Compared to that,
     * wc -l takes 200 ms, so there's room for optimization.
     */
    ChunkHeader ch = {};
    ch.offset = in.pos();

    while (in.readLineInto(nullptr)) {
        m_uTotalLines++;
        ch.lines++;
        if (ch.lines == m_uLinesPerChunk) {
            // TODO: QTextStream::pos() is dead slow and therefore unusuable for our use case !!! see
            // https://www.qtcentre.org/threads/50675-solved-Alternatives-to-the-slow-QTextStream-pos()-method
            // https://stackoverflow.com/questions/30327496/qtextstream-messing-its-position-pointer
            qint64 offset = 666;//in.pos();
            appendChunkHeader(ch, offset);
        }
    }

    appendChunkHeader(ch, 666);

    qDebug() << "Codec:" << in.codec()->name();
}

void LogFile::appendChunkHeader(ChunkHeader& ch, qint64 offset)
{
    if (ch.lines == 0) {
        // ignore empty chunks (can happen at file end)
        return;
    }
    ch.bytes = static_cast<quint32>(offset - ch.offset);
    //qDebug() << m_Index.size() << ch.offset << ch.bytes << ch.lines;
    m_Index.append(ch);
    ch.offset = offset;
    ch.bytes = 0;
    ch.lines = 0;
}
