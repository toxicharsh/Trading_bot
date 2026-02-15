#pragma once
#include <fstream>
#include <mutex>
#include <filesystem>

namespace Analytics
{
    class RiskLogger
    {
        std::ofstream file;
        std::mutex lock;

    public:
        RiskLogger()
        {
            std::filesystem::create_directories("output");
            file.open("output/risk.csv");

            file << "date,symbol,cash,max_alloc,atr,price,final_qty\n";
            file.flush();
        }

        void log(
            const std::string &date,
            const std::string &symbol,
            double cash,
            double max_alloc,
            double atr,
            double price,
            double final_qty)
        {
            std::lock_guard<std::mutex> g(lock);

            file << date << ","
                 << symbol << ","
                 << cash << ","
                 << max_alloc << ","
                 << atr << ","
                 << price << ","
                 << final_qty << "\n";

            file.flush();
        }
    };
}
