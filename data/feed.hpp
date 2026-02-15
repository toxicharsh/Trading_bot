#pragma once
#include "../core/types.hpp"

namespace Data
{

    class IDataFeed
    {
    public:
        virtual ~IDataFeed() = default;
        virtual bool has_next() const = 0;
        virtual Core::Bar next() = 0;
    };

}
