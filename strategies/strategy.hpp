#pragma once
#include "../core/types.hpp"

namespace Strategy
{

    class IStrategy
    {
    public:
        virtual Core::Signal on_bar(
            const Core::Bar &bar,
            double position_qty) = 0;

        virtual ~IStrategy() = default;
    };
}
