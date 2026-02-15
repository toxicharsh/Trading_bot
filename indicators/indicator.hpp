#pragma once

namespace Indicators
{

    class Indicator
    {
    public:
        virtual ~Indicator() = default;

        // Update indicator with new price
        virtual void update(double price) = 0;

        // Latest computed value
        virtual double value() const = 0;

        // True if enough data exists
        virtual bool ready() const = 0;
    };

}
