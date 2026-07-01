#pragma once

#include <cstdint>
#include <string>

/// @brief INI 格式配置解析器，单例加载 config.ini
class ConfigManager {
public:
    struct ServerConfig {
        uint16_t port     = 8080;
        int      num_threads = 0;   // 0 = 自动检测
    };

    struct ModelConfig {
        std::string path            = "./models";
        size_t      max_input_bytes = 10 * 1024 * 1024;  // 10 MB
    };

    struct BatchConfig {
        int max_batch_size       = 32;
        int batch_timeout_ms     = 10;
        int starvation_timeout_ms = 100;
    };

    /// @brief 解析 INI 文件，成功返回 true
    bool load(const std::string& filepath);

    // 便捷输出
    void print() const;

    ServerConfig srv_cfg;
    ModelConfig  mdl_cfg;
    BatchConfig  bat_cfg;
    std::string  log_level = "info";

private:
    static std::string trim(const std::string& s);
};
