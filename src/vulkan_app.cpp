#include "vulkan_app.hpp"
#include "GLFW/glfw3.h"
#include <memory>
#include <stdexcept>

using namespace vulkan_engine;

void VulkanApp::run() {
    while (!engineWindow_.shouldClose()) {
        glfwPollEvents();
    }
}

VulkanApp::VulkanApp() {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

VulkanApp::~VulkanApp() {
    vkDestroyPipelineLayout(device_.getDevice(), pipelineLayout_, nullptr);
}

void VulkanApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(device_.getDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout_) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void VulkanApp::createPipeline() {
    auto pipelineConfig {
        VulkanEnginePipeline::makePipelineConfInfo(swapChain_.width(), swapChain_.height()) 
    };
    pipelineConfig.renderPass = swapChain_.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout_;
    pipeline_ = std::make_unique<VulkanEnginePipeline>(
        device_,
        "shaders/basic_first.vert.spv",
        "shaders/basic_first.frag.spv",
        std::move(pipelineConfig)
    );
}

void VulkanApp::createCommandBuffers() {
}

void VulkanApp::drawFrame() {
}
