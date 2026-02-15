#pragma once
#include "../core/types.hpp"

namespace Execution
{

    class IBroker
    {
    public:
        virtual ~IBroker() = default;
        virtual Core::Fill execute(
            const Core::Order &order,
            const Core::Bar &bar) = 0;
    };

}
