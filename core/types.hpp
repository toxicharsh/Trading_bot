#pragma once
#include <string>

namespace Core
{

    enum class Side
    {
        BUY,
        SELL,
        HOLD
    };

    struct Bar
    {
        std::string date;

        double open;
        double high;
        double low;
        double close;
        double volume;
    };

    struct Signal
    {
        Side direction;
        double confidence;

        // Strategy snapshot for audit logging
        double rsi;
        double atr;
        bool volume_spike;
    };

    struct Order
    {
        std::string symbol;
        Side side;
        double qty;
    };

    struct Fill
    {
        std::string symbol;
        Side side;
        double qty;
        double price;
        double fee;
    };

}
