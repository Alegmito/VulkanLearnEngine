#include "vulkanengine_window.hpp"
#include "GLFW/glfw3.h"

namespace s = std;

using namespace vulkan_engine;
VulkanEngineWindow::VulkanEngineWindow(uint32_t width, uint32_t height, std::string windowName)
    : width{width}, height{height}, windowName{windowName} {
    initWindow();
}
VulkanEngineWindow::~VulkanEngineWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanEngineWindow::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

void VulkanEngineWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface_) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface_) != VK_SUCCESS) {
        throw s::runtime_error("failed to create window surface");
    }
}
