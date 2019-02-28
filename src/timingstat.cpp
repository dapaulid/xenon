#include "timingstat.h"

CTimingStat::CTimingStat():
    m_started(),
    m_uCount(0),
    m_dTotalElapsed(0.0),
    m_dMin(0.0),
    m_dMax(0.0)
{
    start();
}

void CTimingStat::start()
{
    m_started = std::chrono::high_resolution_clock::now();
}

double CTimingStat::stop()
{
    auto stopped = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = stopped - m_started;
    double elapsed = diff.count();
    if (m_uCount == 0) {
        m_dMin = elapsed;
        m_dMax = elapsed;
    } else {
        if (m_dMin > elapsed) m_dMin = elapsed;
        if (m_dMax < elapsed) m_dMax = elapsed;
    }
    m_uCount++;
    m_dTotalElapsed += elapsed;

    return elapsed;
}

