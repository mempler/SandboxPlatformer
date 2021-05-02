#pragma once

#include <EASTL/chrono.h>

class Timer {
public:
    Timer() : beg_(clock_::now()) {
    }
    void reset() {
        beg_ = clock_::now();
    }
    double elapsed() const {
        return eastl::chrono::duration_cast<second_>(clock_::now() - beg_).count();
    }

private:
    typedef eastl::chrono::high_resolution_clock clock_;
    typedef eastl::chrono::duration<double, eastl::ratio<1>> second_;
    eastl::chrono::time_point<clock_> beg_;
};
