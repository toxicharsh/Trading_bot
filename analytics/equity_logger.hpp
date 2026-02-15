#pragma once

#include <fstream>
#include <mutex>
#include <filesystem>
#include <iostream>

namespace Analytics
{

    class EquityLogger
    {
        std::ofstream file;
        std::mutex lock;

    public:
        EquityLogger()
        {
            // ✅ Ensure output folder exists
            std::filesystem::create_directories("output");

            // ✅ Open equity.csv
            file.open("output/equity.csv");

            if (!file.is_open())
            {
                std::cerr << "ERROR: Cannot open output/equity.csv\n";
                return;
            }

            // ✅ Write header
            file << "date,value\n";
            file.flush();
        }

        // ==========================================
        // ✅ MAIN FUNCTION: Log equity each bar
        // ==========================================
        void log(const std::string &date, double equity)
        {
            std::lock_guard<std::mutex> g(lock);

            if (!file.is_open())
                return;

            file << date << "," << equity << "\n";
            file.flush();
        }

        // ==========================================
        // Optional: log final equity snapshot
        // ==========================================
        void log_final(double equity)
        {
            log("FINAL", equity);
        }

        ~EquityLogger()
        {
            if (file.is_open())
            {
                file.flush();
                file.close();
            }
        }
    };

}
