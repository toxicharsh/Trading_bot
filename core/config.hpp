#pragma once
#include <vector>
#include <string>

namespace Core
{

    struct Config
    {

        // Assets
        std::vector<std::string> symbols = {
            "AAPL", "MSFT", "GOOG"};

        // Benchmark index (Sensex)
        std::string benchmark = "^BSESN";

        // Portfolio cash pool
        double initial_cash = 200000;

        // Time window
        std::string start_date = "2020-01-01";
        std::string end_date = "2023-01-01";

        // Execution cost
        double fee_pct = 0.001;

        // Risk sizing
        double max_alloc_per_trade = 0.10;

        // Surgical Scalper params
        int rsi_period = 14;
        double rsi_min = 40;
        double rsi_max = 60;

        int atr_period = 14;
        double min_atr = 1.0;

        int vol_lookback = 20;
        double vol_mult = 2.0;
    };

}
