#include <windows.h>
#include "vulkanengine_pipeline.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace vulkan_engine {
VulkanEnginePipeline::VulkanEnginePipeline(const std::string &vertShaderPath, const std::string &fragShaderPath) {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    std::cout << "current file is " << path << std::endl;
#endif
    createGraphicsPipeline(vertShaderPath, fragShaderPath);
}

std::vector<char> VulkanEnginePipeline::readFile(const std::string path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file " + path);
    }

    size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void VulkanEnginePipeline::createGraphicsPipeline(const std::string &vertShaderPath, const std::string &fragShaderPath) {
    auto vertShaderCode = readFile(vertShaderPath);
    auto fragShaderCode = readFile(fragShaderPath);

    std::cout << "Vertex Shader Code Size: " << vertShaderCode.size();
    std::cout << "Fragment Shader Code Size: " << fragShaderCode.size();
}
}
