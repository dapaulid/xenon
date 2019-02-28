#ifndef TIMINGSTAT_H
#define TIMINGSTAT_H

#include <chrono>

class CTimingStat
{
public:
    CTimingStat();

    void start();
    double stop();

    unsigned long count() const {
        return m_uCount;
    }

    double totalElapsed() const {
        return m_dTotalElapsed;
    }

    double mean() {
        return m_uCount != 0 ? m_dTotalElapsed / m_uCount : 0;
    }

    double min() const {
        return m_dMin;
    }

    double max() const {
        return m_dMax;
    }

protected:
    std::chrono::time_point<std::chrono::system_clock> m_started;
    unsigned long m_uCount;
    double m_dTotalElapsed;
    double m_dMin;
    double m_dMax;
};

#endif // TIMINGSTAT_H
