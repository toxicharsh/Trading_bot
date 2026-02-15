#pragma once

#include <thread>
#include <vector>
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
            // ============================
            // 1️⃣ Shared Portfolio
            // ============================
            Portfolio::Portfolio portfolio(cash);

            // ============================
            // 2️⃣ Shared Loggers (Thread-Safe)
            // ============================
            Analytics::TradeLogger trade_logger;
            Analytics::SignalLogger signal_logger;
            Analytics::RiskLogger risk_logger;

            // Equity logger ONLY used centrally
            Analytics::EquityLogger equity_logger;

            // ============================
            // 3️⃣ Launch Threads
            // ============================
            std::vector<std::thread> threads;

            for (auto &sym : symbols)
            {
                // ✅ Correct path (running from build/)
                std::string csv_path = "data/" + sym + ".csv";

                threads.push_back(std::thread(
                    [&, sym, csv_path]()
                    {
                        TradingEngine engine(
                            sym,
                            csv_path,
                            &portfolio,
                            &trade_logger,
                            &signal_logger,
                            &risk_logger,
                            fee,
                            max_alloc);

                        engine.run(start, end);
                    }));
            }

            // ============================
            // 4️⃣ Wait for All Threads
            // ============================
            for (auto &t : threads)
                t.join();

            // ============================
            // 5️⃣ Final Equity Snapshot (ONE WRITE)
            // ============================
            equity_logger.log_final(portfolio.cash_balance());

            std::cout << "\n=============================\n";
            std::cout << " Backtest Complete\n";
            std::cout << "=============================\n";
            std::cout << "Trades saved  : output/trades.csv\n";
            std::cout << "Signals saved : output/signals.csv\n";
            std::cout << "Risk saved    : output/risk.csv\n";
            std::cout << "Equity saved  : output/equity.csv\n";
        }
    };

}
