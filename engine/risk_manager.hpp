#pragma once

#include "../core/types.hpp"
#include "../portfolio/portfolio.hpp"

namespace Engine
{

    class RiskManager
    {
        double max_alloc;

    public:
        RiskManager(double m)
            : max_alloc(m) {}

        double max_allocation() const
        {
            return max_alloc;
        }

        Core::Order size_order(
            const std::string &symbol,
            const Core::Signal &sig,
            const Core::Bar &bar,
            Portfolio::Portfolio &pf)
        {
            // ============================
            // ✅ BUY: allocate cash %
            // ============================
            if (sig.direction == Core::Side::BUY)
            {
                double cash = pf.cash_balance();
                double invest = cash * max_alloc;

                if (invest <= 0 || bar.close <= 0)
                    return {symbol, Core::Side::HOLD, 0};

                double qty = invest / bar.close;

                if (qty <= 0)
                    return {symbol, Core::Side::HOLD, 0};

                return {symbol, Core::Side::BUY, qty};
            }

            // ============================
            // ✅ SELL: only sell what you own
            // ============================
            if (sig.direction == Core::Side::SELL)
            {
                double held = pf.position_qty(symbol);

                if (held <= 0)
                    return {symbol, Core::Side::HOLD, 0};

                return {symbol, Core::Side::SELL, held};
            }

            return {symbol, Core::Side::HOLD, 0};
        }
    };

}
