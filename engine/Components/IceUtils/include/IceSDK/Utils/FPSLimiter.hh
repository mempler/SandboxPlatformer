
#pragma once

#include <chrono>
#include <ratio>
#include <thread>

// https://stackoverflow.com/a/59165784
namespace IceSDK
{
    class FrameLimiter
    {
      public:
        FrameLimiter( uint32_t FPS )
        {
            m_TimeBetweenFrames =
                std::chrono::microseconds( std::chrono::seconds( 1 ) ) / FPS;

            m_TimePoint = std::chrono::steady_clock::now();
        }

        void Wait()
        {
            // add to time point
            m_TimePoint += m_TimeBetweenFrames;

            // and sleep until that time point
            std::this_thread::sleep_until( m_TimePoint );
        }

      private:
        // a duration with a length of 1/FPS seconds
        std::chrono::duration<long, std::nano> m_TimeBetweenFrames;

        // the time point we'll add to in every loop
        std::chrono::time_point<std::chrono::steady_clock,
                                std::chrono::duration<long, std::nano>>
            m_TimePoint;
    };
}  // namespace IceSDK
