#pragma once
#include <fstream>
#include <mutex>
#include <filesystem>

namespace Analytics
{
    class SignalLogger
    {
        std::ofstream file;
        std::mutex lock;

    public:
        SignalLogger()
        {
            std::filesystem::create_directories("output");
            file.open("output/signals.csv");

            file << "date,symbol,rsi,atr,volume_spike,signal\n";
            file.flush();
        }

        void log(
            const std::string &date,
            const std::string &symbol,
            double rsi,
            double atr,
            bool spike,
            const std::string &signal)
        {
            std::lock_guard<std::mutex> g(lock);

            file << date << ","
                 << symbol << ","
                 << rsi << ","
                 << atr << ","
                 << spike << ","
                 << signal << "\n";

            file.flush();
        }
    };
}
