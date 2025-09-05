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
    commandBuffers_.resize(swapChain_.imageCount());

    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    if (vkAllocateCommandBuffers(device_.getDevice(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    size_t i {};
    for (auto& commandBuffer : commandBuffers_) {
        VkCommandBufferBeginInfo commandBuffInfo {};
        commandBuffInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &commandBuffInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begib recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain_.getRenderPass();
        renderPassInfo.framebuffer = swapChain_.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain_.getSwapChainExtent();

        std::vector<VkClearValue> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
        clearValues[1].depthStencil = {1.0, 0};
        renderPassInfo.clearValueCount = clearValues.size();
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        ++i;
    }
}

void VulkanApp::drawFrame() {
}
