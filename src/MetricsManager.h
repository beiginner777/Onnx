#pragma once

#include <cstdint>
#include <string>

/// @brief Prometheus 指标采集
/// @note  Day 15 实现：prometheus-cpp 集成
class MetricsManager {
public:
    MetricsManager();
    ~MetricsManager();

    // TODO Day 15: void recordInfer(float latency_ms, bool success);
    // TODO Day 15: std::string serialize() const;

    MetricsManager(const MetricsManager&)            = delete;
    MetricsManager& operator=(const MetricsManager&) = delete;

private:
    // TODO Day 15: prometheus counters / histograms / gauges
};
