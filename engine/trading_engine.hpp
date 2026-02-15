#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "../data/csv_feed.hpp"
#include "../strategies/surgical_scalper.hpp"
#include "../execution/broker_sim.hpp"
#include "../portfolio/portfolio.hpp"
#include "../engine/risk_manager.hpp"

#include "../analytics/trade_logger.hpp"
#include "../analytics/signal_logger.hpp"
#include "../analytics/risk_logger.hpp"
#include "../analytics/equity_logger.hpp" // ✅ ADD THIS

namespace Engine
{

    class TradingEngine
    {
        std::string symbol;
        std::string csv_path;

        Strategy::SurgicalScalper strategy;
        Execution::SimBroker broker;
        RiskManager risk;

        Portfolio::Portfolio *portfolio;

        // Loggers
        Analytics::TradeLogger *trade_logger;
        Analytics::SignalLogger *signal_logger;
        Analytics::RiskLogger *risk_logger;
        Analytics::EquityLogger *equity_logger; // ✅ ADD THIS

    public:
        TradingEngine(
            const std::string &sym,
            const std::string &path,
            Portfolio::Portfolio *pf,
            Analytics::TradeLogger *tlog,
            Analytics::SignalLogger *slog,
            Analytics::RiskLogger *rlog,
            Analytics::EquityLogger *elog, // ✅ ADD THIS
            double fee,
            double max_alloc)
            : symbol(sym),
              csv_path(path),
              broker(fee),
              risk(max_alloc),
              portfolio(pf),
              trade_logger(tlog),
              signal_logger(slog),
              risk_logger(rlog),
              equity_logger(elog) // ✅ STORE IT
        {
        }

        void run(const std::string &start,
                 const std::string &end)
        {
            Data::CSVFeed feed;

            if (!feed.load(csv_path, start, end))
            {
                std::cout << "[ENGINE ERROR] Cannot load data: "
                          << csv_path << "\n";
                return;
            }

            while (feed.has_next())
            {
                auto bar = feed.next();

                // ===============================
                // 1️⃣ Current Position
                // ===============================
                double pos_qty = portfolio->position_qty(symbol);

                // ===============================
                // 2️⃣ Strategy Signal
                // ===============================
                auto sig = strategy.on_bar(bar, pos_qty);

                signal_logger->log(
                    bar.date,
                    symbol,
                    sig.rsi,
                    sig.atr,
                    sig.volume_spike,
                    (sig.direction == Core::Side::BUY
                         ? "BUY"
                     : sig.direction == Core::Side::SELL
                         ? "SELL"
                         : "HOLD"));

                // ===============================
                // 3️⃣ Equity Snapshot ✅ LOG IT
                // ===============================
                double equity_before =
                    portfolio->cash_balance() +
                    pos_qty * bar.close;

                equity_logger->log(bar.date, equity_before); // ✅ FIX

                if (sig.direction == Core::Side::HOLD)
                    continue;

                // ===============================
                // 4️⃣ Risk Sizing
                // ===============================
                double cash_before = portfolio->cash_balance();

                auto order = risk.size_order(symbol, sig, bar, *portfolio);

                risk_logger->log(
                    bar.date,
                    symbol,
                    cash_before,
                    risk.max_allocation(),
                    sig.atr,
                    order.qty,
                    order.qty);

                if (order.qty <= 0)
                    continue;

                // ===============================
                // 5️⃣ Execute Trade
                // ===============================
                auto fill = broker.execute(order, bar);

                if (!portfolio->apply_fill(fill))
                    continue;

                double cash_after = portfolio->cash_balance();
                double pos_after = portfolio->position_qty(symbol);

                double equity_after =
                    cash_after + pos_after * bar.close;

                // ===============================
                // 6️⃣ Log Trade
                // ===============================
                trade_logger->log(
                    bar.date,
                    symbol,
                    (fill.side == Core::Side::BUY ? "BUY" : "SELL"),
                    fill.qty,
                    fill.price,
                    fill.fee,
                    cash_before,
                    cash_after,
                    pos_qty,
                    pos_after,
                    equity_before,
                    equity_after);
            }
        }
    };

}
