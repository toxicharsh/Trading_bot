#include "core/config.hpp"
#include "engine/multi_asset_runner.hpp"

int main()
{
    Core::Config cfg;

    Engine::MultiAssetRunner runner(cfg.symbols);

    runner.run_all(
        cfg.initial_cash,
        cfg.fee_pct,
        cfg.max_alloc_per_trade,
        cfg.start_date,
        cfg.end_date);

    return 0;
}
