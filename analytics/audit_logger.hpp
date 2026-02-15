#pragma once
#include <fstream>
#include <mutex>
#include <filesystem>
#include <iostream>

#include "../core/paths.hpp"

namespace Analytics
{
    class AuditLogger
    {
        std::ofstream file;
        std::mutex lock;

    public:
        AuditLogger()
        {
            std::filesystem::create_directories(Core::LOG_DIR);

            file.open(Core::LOG_DIR + "audit.csv");

            if (!file.is_open())
            {
                std::cerr << "ERROR: Cannot open audit.csv\n";
                return;
            }

            file << "date,symbol,side,qty,price,fee,"
                    "cash_before,cash_after,"
                    "rsi,atr,volume_spike\n";

            file.flush();
        }

        void log(
            const std::string &date,
            const std::string &sym,
            const std::string &side,
            double qty,
            double price,
            double fee,
            double cash_before,
            double cash_after,
            double rsi,
            double atr,
            bool spike)
        {
            std::lock_guard<std::mutex> g(lock);

            if (!file.is_open())
                return;

            file << date << ","
                 << sym << ","
                 << side << ","
                 << qty << ","
                 << price << ","
                 << fee << ","
                 << cash_before << ","
                 << cash_after << ","
                 << rsi << ","
                 << atr << ","
                 << spike << "\n";

            file.flush();
        }

        ~AuditLogger()
        {
            if (file.is_open())
            {
                file.flush();
                file.close();
            }
        }
    };
}
