#include <windows.h>
#include "vulkanengine_pipeline.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace s = std;

namespace vulkan_engine {

VulkanEnginePipeline::VulkanEnginePipeline(
    VulkanEngineDevice& device,
    const std::string &vertShaderPath,
    const std::string &fragShaderPath,
    PipelineConfigInfo&& configInfo
) : vkDevice_(device)
{
    createGraphicsPipeline(vertShaderPath, fragShaderPath, configInfo);
}

VulkanEnginePipeline::~VulkanEnginePipeline() {
    vkDestroyShaderModule(vkDevice_.device(), vertShaderModule_, nullptr);
    vkDestroyShaderModule(vkDevice_.device(), fragShaderModule_, nullptr);
    vkDestroyPipeline(vkDevice_.device(), graphicsPipeline_, nullptr);
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

void VulkanEnginePipeline::createGraphicsPipeline(
    const std::string &vertShaderPath, const std::string &fragShaderPath, const PipelineConfigInfo& info
) {
    auto vertShaderCode = readFile(vertShaderPath);
    auto fragShaderCode = readFile(fragShaderPath);

    // std::cout << "Vertex Shader Code Size: " << vertShaderCode.size();
    // std::cout << "Fragment Shader Code Size: " << fragShaderCode.size();
    createShaderModule(vertShaderCode, &vertShaderModule_);
    createShaderModule(fragShaderCode, &fragShaderModule_);

    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShaderModule_;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShaderModule_;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = {};
    vertexInputInfo.vertexBindingDescriptionCount = {};
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkGraphicsPipelineCreateInfo pipelineInfo {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &info.inputAssemblyInfo;
    pipelineInfo.pViewportState = &info.viewportInfo;
    pipelineInfo.pRasterizationState = &info.rasterizationInfo;
    pipelineInfo.pMultisampleState = &info.multisampleInfo;
    pipelineInfo.pColorBlendState = &info.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &info.depthStencilInfo;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = info.pipelineLayout;
    pipelineInfo.renderPass = info.renderPass;
    pipelineInfo.subpass = info.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(vkDevice_.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline successfully");
    }

}

void VulkanEnginePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(vkDevice_.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        throw s::runtime_error("failed to create shader module");
    }
}

PipelineConfigInfo VulkanEnginePipeline::makePipelineConfInfo(uint32_t width, uint32_t height){
    auto info {PipelineConfigInfo{}};
    info.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    info.viewport.x = 0.0f;
    info.viewport.y = 0.0f;
    info.viewport.width = static_cast<float>(width);
    info.viewport.height = static_cast<float>(height);
    info.viewport.minDepth = 0.0f;
    info.viewport.maxDepth = 1.0f;

    info.scissor.offset = {0, 0};
    info.scissor.extent = {width, height};

    info.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.viewportInfo.viewportCount = 1;
    info.viewportInfo.pViewports = &info.viewport;
    info.viewportInfo.scissorCount = 1;
    info.viewportInfo.pScissors = &info.scissor;

    info.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.rasterizationInfo.depthClampEnable = VK_FALSE;
    info.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    info.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    info.rasterizationInfo.lineWidth = 1.0f;
    info.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    info.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    info.rasterizationInfo.depthBiasEnable = VK_FALSE;
    info.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    info.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
    info.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

    info.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.multisampleInfo.sampleShadingEnable = VK_FALSE;
    info.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    info.multisampleInfo.minSampleShading = 1.0f;           // Optional
    info.multisampleInfo.pSampleMask = nullptr;             // Optional
    info.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    info.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

    info.colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    info.colorBlendAttachment.blendEnable = VK_FALSE;
    info.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    info.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    info.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    info.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    info.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    info.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    info.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.colorBlendInfo.logicOpEnable = VK_FALSE;
    info.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    info.colorBlendInfo.attachmentCount = 1;
    info.colorBlendInfo.pAttachments = &info.colorBlendAttachment;
    info.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    info.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    info.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    info.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    info.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    info.depthStencilInfo.depthTestEnable = VK_TRUE;
    info.depthStencilInfo.depthWriteEnable = VK_TRUE;
    info.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    info.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    info.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    info.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    info.depthStencilInfo.stencilTestEnable = VK_FALSE;
    info.depthStencilInfo.front = {};  // Optional
    info.depthStencilInfo.back = {};   // Optional

    return info;
}
}
