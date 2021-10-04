#pragma once

#define PROFILING

#ifdef PROFILING
class Timer
{
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;

public:
    Timer(const char* name)
        : m_Name(name)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

        printf(m_Name, end - start);
    }
};

#define PROFILE_SCOPE(id) Timer timer = Timer(id " %dms\n");
#define PROFILE_SCOPE() Timer timer = Timer(__FUNCTION__ " %dms\n");
#else
#define PROFILE_SCOPE(id)
#define PROFILE()
#endif
