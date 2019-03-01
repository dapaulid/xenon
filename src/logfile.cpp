#include "logfile.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

#include <fstream>
#include <locale>
#include <codecvt>

CLogFile::CLogFile(const QString& asFileName, QObject* parent):
    QObject(parent),
    m_sFileName(asFileName),
    m_uTotalLines(0),
    m_uLinesPerChunk(DEFAULT_LINES_PER_CHUNK),
    m_Index(),
    m_Chunks(MAX_LOADED_CHUNKS_PER_FILE),
    m_pParser(new CLogFileParser()),
    m_CacheStat(),
    m_Timer(),
    m_LastModified()
{
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(timer()));
    load();
    m_Timer.start(DEFAULT_FILE_POLL_INTERVAL_MS);
}

CLogFile::~CLogFile()
{
}

QString CLogFile::getLine(size_t index) const
{
    SLogFileEntry* pEntry = getEntry(index);
    return pEntry ? pEntry->line : QString();
}

QVariant CLogFile::getItem(size_t index, size_t column) const
{
    SLogFileEntry* pEntry = getEntry(index);
    if (pEntry) {
        if (pEntry->columns.empty()) {
            pEntry->columns.resize(m_pParser->getColumnCount());
        }
        if (pEntry->columns[column].isNull()) {
            pEntry->columns[column] = m_pParser->parseLine(pEntry->line, column);
        }
        return pEntry->columns[column];
    } else {
        return QVariant();
    }
}

SLogFileEntry* CLogFile::getEntry(size_t index) const
{
    // get chunk index from entry index
    size_t ci = index / m_uLinesPerChunk;
    size_t ei = index % m_uLinesPerChunk;

    // get chunk
    SLogFileChunk* pChunk = accessChunk(ci);

    // get entry
    return pChunk ? &pChunk->entries[ei] : nullptr;
}

SLogFileChunk* CLogFile::accessChunk(size_t index) const
{
    // chunk already cached?
    SLogFileChunk* pChunk = m_Chunks[index];
    if (!pChunk) {
        // no -> load and add it
        qDebug("cache miss, loading chunk #%lu", index);
        m_CacheStat.start();
        pChunk = loadChunk(index);
        m_Chunks.insert(index, pChunk);
        double elapsed = m_CacheStat.stop();
        qDebug("chunk #%lu loaded, took %0.3f ms (MIN=%0.3f|MEAN=%0.3f|MAX=%0.3f)",
            index, elapsed, m_CacheStat.min(), m_CacheStat.mean(), m_CacheStat.max());
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
        qCritical() << "Failed to open file" << m_sFileName << "for reading chunk";
        return nullptr;
    }

    // start reading at line of chunk
    QTextStream in(&file);
    if (!in.seek(ch.offset)) {
        qDebug() << "Failed to seek to" << ch.offset << "for reading chunk";
        return nullptr;
    }

    // allocate new chunk
    SLogFileChunk* pChunk = new SLogFileChunk;

    // read line by line
    pChunk->entries.reserve(m_uLinesPerChunk);
    for (size_t i = 0; i < ch.lines; i++) {
        SLogFileEntry e = {};
        in.readLineInto(&e.line);
        pChunk->entries.push_back(e);
    }

    // done
    return pChunk;
}

void CLogFile::load()
{
    size_t oldLineCount = m_uTotalLines;

    // discard old stuff
    clear();

    // open file
    std::ifstream in(m_sFileName.toStdString());
    if (!in.good()) {
        qCritical() << "Failed to open" << m_sFileName;
        return;
    }

    SLogFileChunkHeader ch = {};
    ch.offset = in.tellg();

    std::vector<QString> firstLines;
    firstLines.reserve(NUM_LINES_TO_ANALYZE);

    // read line by line
    char line [MAX_LINE_LENGTH];
    while (in.getline(line, sizeof(line))) {

        // index into chunks
        if (ch.lines++ == m_uLinesPerChunk) {
            appendChunkHeader(ch, in.tellg());
        }

        // train our parser
        if (m_uTotalLines < NUM_LINES_TO_ANALYZE) {
            firstLines.push_back(line);
        } else if (m_uTotalLines == NUM_LINES_TO_ANALYZE) {
            m_pParser->prepare(firstLines);
        }

        m_uTotalLines++;
    }

    if (m_uTotalLines < NUM_LINES_TO_ANALYZE) {
        // make the best from what we've got
        m_pParser->prepare(firstLines);
    }

    // finalize index
    in.clear(); // reset error flags for final tellg to work
    appendChunkHeader(ch, in.tellg());

    // init last modification timestamp
    updateLastModified();

    // trace some info
    qInfo() << "Lines:" << m_uTotalLines;
    qInfo() << "Chunks:" << m_Index.size();

    size_t newLineCount = m_uTotalLines;

    // notify observers
    emit changed(oldLineCount, newLineCount);
}

void CLogFile::clear()
{
    m_Index.clear();
    m_Chunks.clear();
    m_uTotalLines = 0;
}

void CLogFile::appendChunkHeader(SLogFileChunkHeader& ch, offset_t offset)
{
    if (ch.lines == 0) {
        // ignore empty chunks (can happen at file end)
        return;
    }
    ch.bytes = static_cast<size_t>(offset - ch.offset);
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

bool CLogFile::updateLastModified()
{
    QFileInfo fi(m_sFileName);

    QDateTime lastModified = fi.lastModified();
    if (m_LastModified != lastModified) {
        m_LastModified = lastModified;
        return true;
    } else {
        return false;
    }
}

void CLogFile::timer()
{
    /*
     * NOTE #1: We use a timer rather than QFileSystemWatcher, because
     * when I checked last (Qt 5.7 under Windows) it was very
     * buggy and catched only modifications after close()ing a file,
     * and not even all of them...
     */
    /*
     * NOTE #2: With our design of lazily loading the file by chunks,
     * it is easier but still efficient to just discard everything
     * and reload the whole file instead of determining what
     * exactly changed.
     */
    if (updateLastModified()) {
        load();
    }
}

QString CLogFile::getDisplayName() const
{
    QFileInfo fi(m_sFileName);
    return fi.fileName();
}
