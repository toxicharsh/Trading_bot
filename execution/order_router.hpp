#pragma once
#include "../core/types.hpp"

namespace Execution
{

    class OrderRouter
    {
    public:
        Core::Order route(const Core::Order &order)
        {
            return order; // placeholder for multi-broker routing
        }
    };

}
