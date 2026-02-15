#pragma once
#include <unordered_map>
#include <mutex>

#include "../core/types.hpp"
#include "position.hpp"

namespace Portfolio
{

    class Portfolio
    {

        double cash;

        // Map: symbol → Position struct
        std::unordered_map<std::string, Position> pos;

        std::mutex lock;

    public:
        Portfolio(double initial)
            : cash(initial) {}

        // ===============================
        // Cash Balance
        // ===============================
        double cash_balance()
        {
            std::lock_guard<std::mutex> g(lock);
            return cash;
        }

        // ===============================
        // Position Quantity
        // ===============================
        double position_qty(const std::string &sym)
        {
            std::lock_guard<std::mutex> g(lock);

            auto it = pos.find(sym);
            if (it == pos.end())
                return 0;

            return it->second.qty;
        }

        // ===============================
        // Apply Fill (BUY/SELL Execution)
        // ===============================
        bool apply_fill(const Core::Fill &f)
        {
            std::lock_guard<std::mutex> g(lock);

            double value = f.qty * f.price;

            // -------- BUY --------
            if (f.side == Core::Side::BUY)
            {
                if (cash < value + f.fee)
                    return false;

                cash -= (value + f.fee);
                pos[f.symbol].qty += f.qty;
            }

            // -------- SELL --------
            if (f.side == Core::Side::SELL)
            {
                if (pos[f.symbol].qty < f.qty)
                    return false;

                cash += (value - f.fee);
                pos[f.symbol].qty -= f.qty;
            }

            return true;
        }

        // ===============================
        // Total Equity (Single Price Debug)
        // ===============================
        double total_equity_simple(double current_price)
        {
            std::lock_guard<std::mutex> g(lock);

            double eq = cash;

            for (auto &kv : pos)
            {
                eq += kv.second.qty * current_price;
            }

            return eq;
        }

        // ===============================
        // ✅ Total Equity (Multi Asset Safe)
        // ===============================
        double total_equity(
            const std::unordered_map<std::string, double> &prices)
        {
            std::lock_guard<std::mutex> g(lock);

            double equity = cash;

            // Loop through all held positions
            for (auto &p : pos)
            {
                const std::string &sym = p.first;
                double qty = p.second.qty;

                // Skip empty positions
                if (qty == 0)
                    continue;

                // Safe lookup of market price
                auto it = prices.find(sym);

                if (it != prices.end())
                {
                    equity += qty * it->second;
                }
            }

            return equity;
        }
    };

}
