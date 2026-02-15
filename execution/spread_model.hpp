#pragma once

namespace Execution
{

    // Abstract Spread Model
    class SpreadModel
    {
    public:
        virtual ~SpreadModel() = default;
        virtual double spread(double mid_price) = 0;
    };

    // Fixed % Spread
    class FixedSpread : public SpreadModel
    {

        double pct;

    public:
        FixedSpread(double p) : pct(p) {}

        double spread(double mid_price) override
        {
            return mid_price * pct;
        }
    };

}
