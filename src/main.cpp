#include "ConfigManager.h"

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <atomic>
#include <csignal>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char* argv[]) {
    // ---- 1. 加载配置 --------------------------------------------------
    std::string config_path = (argc > 1) ? argv[1] : "config.ini";

    ConfigManager cfg;
    if (!cfg.load(config_path)) {
        spdlog::critical("Failed to load config from: {}", config_path);
        return 1;
    }

    // 设置日志级别
    spdlog::set_level(spdlog::level::from_str(cfg.log_level));

    spdlog::info("╔══════════════════════════════════════════╗");
    spdlog::info("║   ONNX Inference Server  v1.0.0          ║");
    spdlog::info("╚══════════════════════════════════════════╝");
    cfg.print();

    // ---- 2. IO Context Pool ------------------------------------------
    int num_threads = cfg.srv_cfg.num_threads;
    if (num_threads <= 0) {
        num_threads = static_cast<int>(std::thread::hardware_concurrency());
    }
    spdlog::info("IO Context Pool: {} threads (hw_concurrency={})",
                 num_threads, std::thread::hardware_concurrency());

    asio::io_context ioc{num_threads > 1 ? num_threads : 1};
    auto work_guard = asio::make_work_guard(ioc);

    // ---- 3. 信号处理 (优雅关闭) --------------------------------------
    asio::signal_set signals(ioc, SIGINT, SIGTERM);
#ifdef SIGQUIT
    signals.add(SIGQUIT);
#endif

    std::atomic<bool> running{true};

    signals.async_wait([&](std::error_code ec, int sig) {
        if (!ec) {
            spdlog::warn("Signal {} received — initiating graceful shutdown...", sig);
            running = false;
            work_guard.reset();   // 允许 io_context 退出
            ioc.stop();           // 唤醒所有阻塞线程
        }
    });

    // ---- 4. 启动 IO 线程 ---------------------------------------------
    std::vector<std::thread> io_threads;
    io_threads.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        io_threads.emplace_back([&ioc, i]() {
            spdlog::debug("IO thread-{} started", i);
            ioc.run();
            spdlog::debug("IO thread-{} exited", i);
        });
    }

    spdlog::info("Server is running.  Press Ctrl+C to stop.");
    spdlog::info("(HTTP endpoints will be wired in Day 2)");

    // ---- 5. 等待线程退出 ---------------------------------------------
    for (auto& t : io_threads) {
        if (t.joinable()) t.join();
    }

    spdlog::info("All threads joined.  Server shut down gracefully.");
    return 0;
}
