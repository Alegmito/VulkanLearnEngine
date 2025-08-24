#pragma once

#include "vulkanengine_device.hpp"
#include "vulkanengine_pipeline.hpp"
#include "vulkanengine_swap_chain.hpp"
#include "vulkanengine_window.hpp"
#include <memory>

namespace vulkan_engine {
class VulkanApp {

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    VulkanApp();
    ~VulkanApp();

    VulkanApp(const VulkanApp &) = delete;
    VulkanApp &operator=(const VulkanApp &) = delete;

    void run();
private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    VulkanEngineWindow engineWindow_ {WIDTH, HEIGHT, "Hello Vulkan!"};
    VulkanEngineDevice device_ {engineWindow_};
    VulkanEngineSwapChain swapChain_ {device_, engineWindow_.getExtent()};
    std::unique_ptr<VulkanEnginePipeline> pipeline_;
    VkPipelineLayout pipelineLayout_;
    std::vector<VkCommandBuffer> commandBuffers_;
};
}
