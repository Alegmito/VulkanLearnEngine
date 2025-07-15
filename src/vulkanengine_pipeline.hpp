#pragma once
#include "vulkanengine_device.hpp"
#include <string>
#include <vector>

namespace vulkan_engine {
struct PipelineConfigInfo {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class VulkanEnginePipeline {
public:
    VulkanEnginePipeline(
        VulkanEngineDevice& device,
        const std::string &vertShaderPath,
        const std::string &fragShaderPath,
        PipelineConfigInfo&& configInfo
    );

    ~VulkanEnginePipeline();

    VulkanEnginePipeline(const VulkanEnginePipeline&) = delete;
    void operator =(const VulkanEnginePipeline&) = delete;

    static PipelineConfigInfo makePipelineConfInfo(uint32_t width, uint32_t height);

private:
    static std::vector<char> readFile(const std::string path);
    void createGraphicsPipeline(
        const std::string &vertShaderPath, const std::string &fragShaderPath, const PipelineConfigInfo& info
    );

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    VulkanEngineDevice& vkDevice_;
    VkPipeline graphicsPipeline_;
    VkShaderModule vertShaderModule_;
    VkShaderModule fragShaderModule_;
};
}
