#pragma once
#include "../core/types.hpp"

namespace Data
{

    class DataFilter
    {
    public:
        static bool valid(const Core::Bar &bar)
        {

            if (bar.close <= 0)
                return false;
            if (bar.volume <= 0)
                return false;
            if (bar.high < bar.low)
                return false;

            return true;
        }
    };

}
