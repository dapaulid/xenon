#include "logfile.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

#include <fstream>
#include <locale>
#include <codecvt>

CLogFile::CLogFile(const QString& asFileName):
    m_sFileName(asFileName),
    m_uTotalLines(0),
    m_uLinesPerChunk(DEFAULT_LINES_PER_CHUNK),
    m_Index(),
    m_Chunks(MAX_LOADED_CHUNKS_PER_FILE),
    m_pParser(new CLogFileParser())
{
    analyze();
}

CLogFile::~CLogFile()
{
}

SLogFileEntry* CLogFile::getEntry(size_t index) const
{
    // get chunk index from entry index
    size_t ci = index / m_uLinesPerChunk;
    size_t ei = index % m_uLinesPerChunk;

    // get chunk
    SLogFileChunk* pChunk = accessChunk(ci);

    // get entry
    return &pChunk->entries[ei];
}

SLogFileChunk* CLogFile::accessChunk(size_t index) const
{
    // chunk already cached?
    SLogFileChunk* pChunk = m_Chunks[index];
    if (!pChunk) {
        // no -> load and add it
        pChunk = loadChunk(index);
        m_Chunks.insert(index, pChunk);
        qDebug() << "Cache miss: Chunk #" << index << "loaded";
    }
    return pChunk;
}

SLogFileChunk* CLogFile::loadChunk(size_t index) const
{
    // get chunk header
    const SLogFileChunkHeader& ch = m_Index[index];

    // open file
    QFile file(m_sFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "fuck";
        return nullptr;
    }

    // start reading at line of chunk
    QTextStream in(&file);
    if (!in.seek(ch.offset)) {
        qDebug() << "Failed to seek to " << ch.offset;
        return nullptr;
    }

    // allocate new chunk
    SLogFileChunk* pChunk = new SLogFileChunk;

    // read line by line
    for (size_t i = 0; i < ch.lines; i++) {
        SLogFileEntry e = {};
        e.line = in.readLine();
        m_pParser->parseEntry(e);
        pChunk->entries.push_back(e);
    }

    // done
    return pChunk;
}

void CLogFile::analyze()
{
    //analyze_utf8();
    analyze_ascii();   // fastest, but there may be issues on utf-8 files (false line breaks?)
    //analyze_generic(); // additionally finds codec, but broken QTextStream::pos()

    qDebug() << "Lines:" << m_uTotalLines;
    qDebug() << "Chunks:" << m_Index.size();
}

void CLogFile::analyze_ascii()
{
    std::ifstream in(m_sFileName.toStdString());

    if (!in.good()) {
        qDebug() << "Failed to open " << m_sFileName;
        return;
    }

    SLogFileChunkHeader ch = {};
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

void CLogFile::analyze_utf8()
{
    std::locale old_locale;
    std::locale utf8_locale(old_locale, new std::codecvt_utf8<wchar_t,0x10ffff, std::consume_header>);

    std::wifstream in(m_sFileName.toStdString());
    in.imbue(std::locale(utf8_locale));

    if (!in.good()) {
        qDebug() << "Failed to open " << m_sFileName;
        return;
    }

    SLogFileChunkHeader ch = {};
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

void CLogFile::analyze_generic()
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
    SLogFileChunkHeader ch = {};
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

void CLogFile::appendChunkHeader(SLogFileChunkHeader& ch, offset_t offset)
{
    if (ch.lines == 0) {
        // ignore empty chunks (can happen at file end)
        return;
    }
    ch.bytes = static_cast<quint32>(offset - ch.offset);
    //qDebug() << m_Index.size() << ch.offset << ch.bytes << ch.lines;
    m_Index.push_back(ch);
    ch.offset = offset;
    ch.bytes = 0;
    ch.lines = 0;
}

size_t CLogFile::getColumnCount() const
{
    return m_pParser->getColumnCount();
}

QString CLogFile::getColumnName(size_t index) const
{
    return m_pParser->getColumnName(index);
}
