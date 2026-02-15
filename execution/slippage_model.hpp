#pragma once

namespace Execution
{

    // Abstract Slippage Model
    class SlippageModel
    {
    public:
        virtual ~SlippageModel() = default;
        virtual double slippage(double price, double qty) = 0;
    };

    // Fixed % Slippage
    class FixedSlippage : public SlippageModel
    {

        double pct;

    public:
        FixedSlippage(double p) : pct(p) {}

        double slippage(double price, double qty) override
        {
            (void)qty; // unused for now
            return price * pct;
        }
    };

}
