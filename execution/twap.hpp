#pragma once
#include <vector>
#include "../core/types.hpp"

namespace Execution
{

    class TWAP
    {

        int slices;

    public:
        TWAP(int n) : slices(n) {}

        std::vector<Core::Order> slice(const Core::Order &parent)
        {

            std::vector<Core::Order> children;

            double piece_qty = parent.qty / slices;

            for (int i = 0; i < slices; i++)
            {

                children.push_back(Core::Order{
                    parent.symbol,
                    parent.side,
                    piece_qty});
            }

            return children;
        }
    };

}
