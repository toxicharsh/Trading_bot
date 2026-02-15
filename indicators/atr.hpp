#pragma once
#include "../core/ring_buffer.hpp"
#include "../core/types.hpp"
#include <cmath>

namespace Indicators
{

    class ATR
    {

        int period;
        Core::RingBuffer<Core::Bar, 128> bars;

    public:
        ATR(int p) : period(p) {}

        void update(const Core::Bar &b) { bars.push(b); }

        bool ready() const
        {
            return bars.size() >= period + 1;
        }

        double value() const
        {

            double sumTR = 0;

            for (int i = 1; i <= period; i++)
            {

                auto cur = bars.get(i);
                auto prev = bars.get(i - 1);

                double tr = std::max({cur.high - cur.low,
                                      std::abs(cur.high - prev.close),
                                      std::abs(cur.low - prev.close)});

                sumTR += tr;
            }

            return sumTR / period;
        }
    };

}
