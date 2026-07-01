#pragma once

#include <cstdint>
#include <functional>
#include <vector>

/// @brief 动态批处理器 — 无锁队列 + 攒批推理
/// @note  Day 8-9 实现：moodycamel::ConcurrentQueue + 动态 batch size
class BatchProcessor {
public:
    BatchProcessor();
    ~BatchProcessor();

    // TODO Day 8: void enqueue(InferRequest req);
    // TODO Day 8: void start(int max_batch, int timeout_ms, int starvation_ms);
    // TODO Day 8: void stop();

    BatchProcessor(const BatchProcessor&)            = delete;
    BatchProcessor& operator=(const BatchProcessor&) = delete;

private:
    // TODO Day 8: moodycamel::ConcurrentQueue<InferRequest> queue_;
    // TODO Day 8: std::thread worker_;
};
