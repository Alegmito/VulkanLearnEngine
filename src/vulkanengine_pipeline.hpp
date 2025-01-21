#pragma once
#include <string>
#include <vector>

namespace vulkan_engine {
class VulkanEnginePipeline {
public:
    VulkanEnginePipeline(const std::string &vertShaderPath, const std::string &fragShaderPath);
private:
    static std::vector<char> readFile(const std::string path);
    void createGraphicsPipeline(const std::string &vertShaderPath, const std::string &fragShaderPath);
};
}
