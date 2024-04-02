#include "windows_lv.h"

lve::Windows_lv::Windows_lv(int w, int h, const std::string& wN) : width(w), height(h), windowName(wN) { initWindow(); }


void lve::Windows_lv::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
}

GLFWwindow* lve::Windows_lv::getWindow() const
{
	return window;
}

VkExtent2D lve::Windows_lv::getExtent()
{
	return {  static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

bool lve::Windows_lv::wasWindowsResize() const
{
	return frameBufferResized;
}

void lve::Windows_lv::resetWindowResizedFlag()
{
	frameBufferResized = false;
}

lve::Windows_lv::~Windows_lv()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool lve::Windows_lv::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void lve::Windows_lv::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

void lve::Windows_lv::frameBufferResizeCallback(GLFWwindow* windo, int w, int h)
{
	Windows_lv* wind = reinterpret_cast<Windows_lv*>(glfwGetWindowUserPointer(windo));
	wind->frameBufferResized = true;
	wind->width = w;
	wind->height = h;
}
