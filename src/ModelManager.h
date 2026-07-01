#pragma once

#include <memory>
#include <string>
#include <unordered_map>

/// @brief 多模型管理与热加载
/// @note  Day 4 实现模型加载；Day 10-11 实现热加载 (shared_ptr + atomic)
class ModelManager {
public:
    ModelManager();
    ~ModelManager();

    // TODO Day 4: bool loadAll(const std::string& model_dir);
    // TODO Day 4: bool exists(const std::string& name) const;
    // TODO Day 10: bool reload(const std::string& name, const std::string& version);

    ModelManager(const ModelManager&)            = delete;
    ModelManager& operator=(const ModelManager&) = delete;

private:
    // TODO Day 4: std::unordered_map<std::string, std::shared_ptr<ModelInfo>> models_;
};
