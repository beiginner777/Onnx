#include "ConfigManager.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

// ---- helpers ------------------------------------------------------------
std::string ConfigManager::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ---- load ---------------------------------------------------------------
bool ConfigManager::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        spdlog::error("Cannot open config file: {}", filepath);
        return false;
    }

    std::string line;
    std::string section;
    int         line_no = 0;

    while (std::getline(file, line)) {
        ++line_no;
        line = trim(line);

        // 空行 / 注释
        if (line.empty() || line[0] == '#' || line[0] == ';')
            continue;

        // [section]
        if (line.front() == '[' && line.back() == ']') {
            section = trim(line.substr(1, line.size() - 2));
            continue;
        }

        // key = value
        auto eq = line.find('=');
        if (eq == std::string::npos) {
            spdlog::warn("config.ini:{} — skip malformed line", line_no);
            continue;
        }

        std::string key   = trim(line.substr(0, eq));
        std::string value = trim(line.substr(eq + 1));

        try {
            if (section == "server") {
                if (key == "port")          srv_cfg.port        = static_cast<uint16_t>(std::stoi(value));
                else if (key == "num_threads") srv_cfg.num_threads = std::stoi(value);
            }
            else if (section == "model") {
                if (key == "path")             mdl_cfg.path            = value;
                else if (key == "max_input_bytes") mdl_cfg.max_input_bytes = std::stoull(value);
            }
            else if (section == "batch") {
                if (key == "max_batch_size")        bat_cfg.max_batch_size        = std::stoi(value);
                else if (key == "batch_timeout_ms")     bat_cfg.batch_timeout_ms     = std::stoi(value);
                else if (key == "starvation_timeout_ms") bat_cfg.starvation_timeout_ms = std::stoi(value);
            }
            else if (section == "log") {
                if (key == "level") log_level = value;
            }
        } catch (const std::exception& e) {
            spdlog::error("config.ini:{} — bad value for '{}': {}", line_no, key, e.what());
        }
    }

    return true;
}

// ---- print --------------------------------------------------------------
void ConfigManager::print() const {
    spdlog::info("┌─ Server ─────────────────────────────────");
    spdlog::info("│ port        : {}", srv_cfg.port);
    spdlog::info("│ num_threads : {} (0=auto)", srv_cfg.num_threads);
    spdlog::info("├─ Model ─────────────────────────────────");
    spdlog::info("│ path            : {}", mdl_cfg.path);
    spdlog::info("│ max_input_bytes : {} bytes", mdl_cfg.max_input_bytes);
    spdlog::info("├─ Batch ─────────────────────────────────");
    spdlog::info("│ max_batch_size       : {}", bat_cfg.max_batch_size);
    spdlog::info("│ batch_timeout_ms     : {} ms", bat_cfg.batch_timeout_ms);
    spdlog::info("│ starvation_timeout_ms: {} ms", bat_cfg.starvation_timeout_ms);
    spdlog::info("├─ Log ───────────────────────────────────");
    spdlog::info("│ level : {}", log_level);
    spdlog::info("└─────────────────────────────────────────");
}
