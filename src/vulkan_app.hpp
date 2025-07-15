#pragma once

#include "vulkanengine_device.hpp"
#include "vulkanengine_pipeline.hpp"
#include "vulkanengine_window.hpp"

namespace vulkan_engine {
class VulkanApp {

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();
private:
    VulkanEngineWindow engineWindow_ {WIDTH, HEIGHT, "Hello Vulkan!"};
    VulkanEngineDevice device_ {engineWindow_};
    VulkanEnginePipeline enginePipeline_ {
        device_ , 
        "./shaders/basic_first.vert.spv", 
        "src/shaders/basic_first.frag.spv", 
        VulkanEnginePipeline::makePipelineConfInfo(WIDTH, HEIGHT)
    };
};
}
