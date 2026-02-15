#pragma once

#include "strategy.hpp"
#include "../indicators/sma.hpp"
#include <algorithm>

namespace Strategy
{

    class SMACrossoverStrategy : public IStrategy
    {

        Indicators::SMA sma_short;
        Indicators::SMA sma_long;

        int last_state;

        double min_strength;
        int cooldown_bars;
        int cooldown_counter;

    public:
        SMACrossoverStrategy(
            int s = 20,
            int l = 50,
            double strength = 0.005,
            int cooldown = 3)
            : sma_short(s),
              sma_long(l),
              last_state(0),
              min_strength(strength),
              cooldown_bars(cooldown),
              cooldown_counter(0)
        {
        }

        Core::Signal on_bar(const Core::Bar &bar) override
        {

            sma_short.update(bar.close);
            sma_long.update(bar.close);

            if (!sma_long.ready())
                return {Core::Side::HOLD, 0};

            if (cooldown_counter > 0)
            {
                cooldown_counter--;
                return {Core::Side::HOLD, 0};
            }

            double s = sma_short.value();
            double l = sma_long.value();

            double strength =
                std::clamp((s - l) / l, 0.0, 1.0);

            int state = (s > l) ? +1 : -1;

            if (state != last_state && strength >= min_strength)
            {

                cooldown_counter = cooldown_bars;
                last_state = state;

                if (state == +1)
                    return {Core::Side::BUY, strength};
                else
                    return {Core::Side::SELL, 1.0 - strength};
            }

            last_state = state;
            return {Core::Side::HOLD, 0};
        }
    };

}
