#pragma once
#include <fstream>
#include <mutex>
#include <filesystem>
#include <iostream>

namespace Analytics
{
    class TradeLogger
    {
        std::ofstream file;
        std::mutex lock;

    public:
        TradeLogger()
        {
            std::filesystem::create_directories("output");
            file.open("output/trades.csv");

            file << "date,symbol,side,qty,price,fee,"
                    "cash_before,cash_after,"
                    "pos_before,pos_after,"
                    "equity_before,equity_after\n";
            file.flush();
        }

        void log(
            const std::string &date,
            const std::string &symbol,
            const std::string &side,
            double qty,
            double price,
            double fee,
            double cash_before,
            double cash_after,
            double pos_before,
            double pos_after,
            double equity_before,
            double equity_after)
        {
            std::lock_guard<std::mutex> g(lock);

            file << date << ","
                 << symbol << ","
                 << side << ","
                 << qty << ","
                 << price << ","
                 << fee << ","
                 << cash_before << ","
                 << cash_after << ","
                 << pos_before << ","
                 << pos_after << ","
                 << equity_before << ","
                 << equity_after << "\n";

            file.flush();
        }
    };
}
