#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <unordered_map>

template<typename T>
class CHistogram
{
public:
    CHistogram(): m_Map() {}

    void insert(const T& entry) {
        m_Map[entry]++;
    }

    size_t count(const T& entry) const {
        auto it = m_Map.find(entry);
        return it != m_Map.end() ? it->second : 0;
    }

    T mode() const {
        if (m_Map.empty()) {
            return T();
        }
        auto max = m_Map.cbegin();
        for (auto it = m_Map.cbegin(); it != m_Map.cend(); it++) {
            if (max->second < it->second) {
                max = it;
            }
        }
        return max->first;
    }

protected:
    std::unordered_map<T, size_t> m_Map;
};

#endif // HISTOGRAM_H
