#pragma once
#include <vector>

namespace Analytics
{

    class Benchmark
    {

        std::vector<double> prices;

    public:
        Benchmark(const std::vector<double> &p)
            : prices(p) {}

        double total_return() const
        {
            return (prices.back() / prices.front()) - 1.0;
        }
    };

}
