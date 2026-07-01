#pragma once

#include <string>
#include <vector>

/// @brief ONNX Runtime 推理封装
/// @note  Day 3 实现：封装 Ort::Session，单次推理
class InferManager {
public:
    InferManager();
    ~InferManager();

    // TODO Day 3: bool loadModel(const std::string& path);
    // TODO Day 3: std::vector<float> run(const std::vector<float>& input);

    InferManager(const InferManager&)            = delete;
    InferManager& operator=(const InferManager&) = delete;

private:
    // TODO Day 3: std::unique_ptr<Ort::Session> session_;
};
