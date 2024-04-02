#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "App.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>


int main() {

    try
    {
        App app(800, 600, "app", true);
        app.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}