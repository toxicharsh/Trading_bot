#pragma once
#include "../core/ring_buffer.hpp"
#include <cmath>

namespace Indicators
{

    class RSI
    {

        int period;
        Core::RingBuffer<double, 128> closes;

    public:
        RSI(int p) : period(p) {}

        void update(double c) { closes.push(c); }

        bool ready() const
        {
            return closes.size() >= period + 1;
        }

        double value() const
        {

            double gain = 0, loss = 0;

            for (int i = 1; i <= period; i++)
            {
                double diff = closes.get(i) - closes.get(i - 1);

                if (diff > 0)
                    gain += diff;
                else
                    loss -= diff;
            }

            gain /= period;
            loss /= period;

            if (loss == 0)
                return 100;
            double rs = gain / loss;

            return 100 - (100 / (1 + rs));
        }
    };

}
