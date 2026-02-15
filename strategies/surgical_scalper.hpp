#pragma once

#include "strategy.hpp"
#include "../indicators/rsi.hpp"
#include "../indicators/atr.hpp"
#include "../indicators/volume_spike.hpp"

namespace Strategy
{

    class SurgicalScalper : public IStrategy
    {
        Indicators::RSI rsi;
        Indicators::ATR atr;
        Indicators::VolumeSpike vol;

        // Entry + Exit thresholds
        double entry_rsi; // Oversold BUY
        double exit_rsi;  // Recovery SELL
        double min_atr;

    public:
        SurgicalScalper()
            : rsi(14),
              atr(14),
              vol(20, 2.0),

              // ✅ Better scalper thresholds
              entry_rsi(35), // BUY if RSI < 35
              exit_rsi(60),  // SELL if RSI > 60

              min_atr(0.2)
        {
        }

        // ✅ FIXED: takes current position qty
        Core::Signal on_bar(const Core::Bar &bar, double position_qty) override
        {
            // Update indicators
            rsi.update(bar.close);
            atr.update(bar);
            vol.update(bar.volume);

            // Not ready → HOLD
            if (!rsi.ready() || !atr.ready() || !vol.ready())
            {
                return {Core::Side::HOLD, 0, 0, 0, false};
            }

            double rv = rsi.value();
            double av = atr.value();
            bool spike = vol.spike();

            // ATR filter (skip dead markets)
            if (av < min_atr)
            {
                return {Core::Side::HOLD, 0, rv, av, spike};
            }

            // ======================================
            // ✅ ENTRY: BUY only when oversold
            // ======================================
            if (position_qty == 0)
            {
                if (rv < entry_rsi)
                {
                    return {Core::Side::BUY, 1.0, rv, av, spike};
                }
            }

            // ======================================
            // ✅ EXIT: SELL when RSI recovers
            // ======================================
            if (position_qty > 0)
            {
                if (rv > exit_rsi)
                {
                    return {Core::Side::SELL, 1.0, rv, av, spike};
                }
            }

            return {Core::Side::HOLD, 0, rv, av, spike};
        }
    };

}
