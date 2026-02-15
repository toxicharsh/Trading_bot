#pragma once
#include "../core/ring_buffer.hpp"

namespace Indicators
{
    class VolumeSpike
    {
        int lookback;
        double mult;
        Core::RingBuffer<double, 256> vols;

    public:
        VolumeSpike(int lb, double m)
            : lookback(lb), mult(m) {}

        void update(double v)
        {
            vols.push(v);
        }

        bool ready() const
        {
            return vols.size() >= lookback;
        }

        bool spike() const
        {
            double avg = 0;
            for (int i = 1; i <= lookback; i++)
                avg += vols.get(i);
            avg /= lookback;
            double current = vols.get(0);
            return current > avg * mult;
        }
    };
}
