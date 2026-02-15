#pragma once
#include "broker.hpp"

namespace Execution
{

    class SimBroker : public IBroker
    {

        double fee_rate;

    public:
        SimBroker(double fee)
            : fee_rate(fee) {}

        Core::Fill execute(
            const Core::Order &o,
            const Core::Bar &bar) override
        {

            double value = o.qty * bar.close;
            double fee = value * fee_rate;

            return {o.symbol, o.side, o.qty, bar.close, fee};
        }
    };

}
