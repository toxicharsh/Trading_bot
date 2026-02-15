#pragma once

#include "feed.hpp"
#include "data_filter.hpp"

#include <vector>
#include <fstream>
#include <sstream>

namespace Data
{

    class CSVFeed : public IDataFeed
    {

        std::vector<Core::Bar> bars;
        int index = 0;

    public:
        bool load(const std::string &file,
                  const std::string &start,
                  const std::string &end)
        {

            std::ifstream f(file);
            if (!f.is_open())
                return false;

            std::string line;
            std::getline(f, line);

            while (std::getline(f, line))
            {

                std::stringstream ss(line);
                std::string token;

                try
                {
                    std::getline(ss, token, ',');
                    std::string date = token;

                    if (date < start || date > end)
                        continue;

                    std::getline(ss, token, ',');
                    double open = std::stod(token);
                    std::getline(ss, token, ',');
                    double high = std::stod(token);
                    std::getline(ss, token, ',');
                    double low = std::stod(token);
                    std::getline(ss, token, ',');
                    double close = std::stod(token);

                    std::getline(ss, token, ','); // adj close skip
                    std::getline(ss, token, ',');
                    double volume = std::stod(token);

                    Core::Bar bar{date, open, high, low, close, volume};

                    if (DataFilter::valid(bar))
                        bars.push_back(bar);
                }
                catch (...)
                {
                    continue;
                }
            }

            return true;
        }

        bool has_next() const override
        {
            return index < (int)bars.size();
        }

        Core::Bar next() override
        {
            return bars[index++];
        }
    };

}
