#include "vulkan_app.hpp"
#include "GLFW/glfw3.h"

namespace vulkan_engine {

void VulkanApp::run() {
    while (!engineWindow_.shouldClose()) {
        glfwPollEvents();
    }
}
}
