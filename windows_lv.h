#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

namespace lve {
	class Windows_lv
	{
		private:
			GLFWwindow* window;
			int width;
			int height;
			bool frameBufferResized = false;
			std::string windowName;
			void initWindow();
			static void frameBufferResizeCallback(GLFWwindow*, int, int);
		public:
			Windows_lv(int = 800, int = 600, const std::string& = "defaultWindow");
			~Windows_lv();
			bool shouldClose();
			void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
			GLFWwindow* getWindow() const;
			VkExtent2D getExtent();
			bool wasWindowsResize() const;
			void resetWindowResizedFlag();
	};
}
