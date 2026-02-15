#pragma once

#include <thread>
#include <vector>
#include <memory>
#include <iostream>

#include "../portfolio/portfolio.hpp"
#include "../analytics/trade_logger.hpp"
#include "../analytics/signal_logger.hpp"
#include "../analytics/risk_logger.hpp"
#include "../analytics/equity_logger.hpp"

#include "trading_engine.hpp"

namespace Engine
{

    class MultiAssetRunner
    {
        std::vector<std::string> symbols;

    public:
        MultiAssetRunner(const std::vector<std::string> &s)
            : symbols(s) {}

        void run_all(
            double cash,
            double fee,
            double max_alloc,
            const std::string &start,
            const std::string &end)
        {
            Portfolio::Portfolio portfolio(cash);

            Analytics::TradeLogger trade_logger;
            Analytics::SignalLogger signal_logger;
            Analytics::RiskLogger risk_logger;
            Analytics::EquityLogger equity_logger;

            std::vector<std::thread> threads;

            for (auto &sym : symbols)
            {
                // ✅ Correct relative path from build/
                std::string csv_path = "data/" + sym + ".csv";

                threads.push_back(std::thread([&, sym, csv_path]()
                                              {
                    TradingEngine engine(
                        sym,
                        csv_path,
                        &portfolio,
                        &trade_logger,
                        &signal_logger,
                        &risk_logger,
                        &equity_logger,
                        fee,
                        max_alloc);

                    engine.run(start, end); }));
            }

            for (auto &t : threads)
                t.join();

            // equity_logger.log_final(portfolio.cash_balance());
        }
    };

}
