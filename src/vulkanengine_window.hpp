#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>
#include <vulkan/vulkan.hpp>

namespace vulkan_engine {
class VulkanEngineWindow {
public:
    VulkanEngineWindow(uint32_t width, uint32_t height, std::string windowName);
    ~VulkanEngineWindow();
    VulkanEngineWindow(const VulkanEngineWindow &) = delete;
    VulkanEngineWindow &operator=(const VulkanEngineWindow &) = delete;
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface_); 

    bool shouldClose() {return glfwWindowShouldClose(window);}
private:
    void initWindow();

    const uint32_t width;
    const uint32_t height;

    std::string windowName;
    GLFWwindow *window;
};
}
