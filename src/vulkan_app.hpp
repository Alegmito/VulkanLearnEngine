#pragma once

#include "vulkanengine_pipeline.hpp"
#include "vulkanengine_window.hpp"

namespace vulkan_engine {
class VulkanApp {

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();
private:
    VulkanEngineWindow engineWindow {WIDTH, HEIGHT, "Hello Vulkan!"};
    VulkanEnginePipeline enginePipeline {"./shaders/basic_first.vert.spv", "src/shaders/basic_first.frag.spv"};
};

}
