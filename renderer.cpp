#include "renderer.h"
#include <cassert>


lve::Renderer::Renderer(lve::Windows_lv& wind, lve::EngineDevice& dev) :window(wind), device(dev)
{
	isFrameStarted = false;
	currentFrameIndex = 0;
	recreateSwapChain();
	createCommandBuffer();
}

lve::Renderer::~Renderer()
{
	freeCommandBuffers();
}

void lve::Renderer::freeCommandBuffers()
{
	vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}

void lve::Renderer::createCommandBuffer()
{
	commandBuffers.resize(swapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to create command buffer");
}

void lve::Renderer::recreateSwapChain()
{
	auto extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = window.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(device.device());

	if (!swapChain)
		swapChain = std::make_unique<lve::EngineSwapChain>(device, extent);
	else {
		std::shared_ptr<EngineSwapChain> oldswapChain = std::move(swapChain);
		swapChain = std::make_unique<lve::EngineSwapChain>(device, extent, oldswapChain);

		if (!oldswapChain->compareSwapFormats(*swapChain.get()))
			throw std::runtime_error("swap chain has change");

		if (swapChain->imageCount() != commandBuffers.size())
		{
			freeCommandBuffers();
			createCommandBuffer();
		}
	}
}

VkCommandBuffer lve::Renderer::beginFrame()
{
	auto result = swapChain->acquireNextImage(&currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return nullptr;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("failed to draw 1");

	isFrameStarted = true;
	auto commandBuffer = getCurrentCommandBuffer();

	VkCommandBufferBeginInfo beingInfo{};
	beingInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(commandBuffer, &beingInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer");

	return commandBuffer;
}

void lve::Renderer::endFrame()
{
	auto commandBuffer = getCurrentCommandBuffer();
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("faiiled to record command buffer");

	auto result = swapChain->submitCommandBuffers(&commandBuffers[currentImageIndex], &currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowsResize())
	{
		window.resetWindowResizedFlag();
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
		throw std::runtime_error("failed to draw 2");

	isFrameStarted = false;
	currentFrameIndex = (currentFrameIndex + 1) % EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void lve::Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0, 0 , 0 , 0 };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

}

void lve::Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

bool lve::Renderer::isFrameInProgress() const
{
	return isFrameStarted;
}

VkCommandBuffer lve::Renderer::getCurrentCommandBuffer() const
{
	return commandBuffers[currentImageIndex];
}

VkRenderPass lve::Renderer::getSwapChainRenderPass() const
{
	return swapChain->getRenderPass();
}

float lve::Renderer::getAspectRation() const
{
	return swapChain->extentAspectRatio();
}

int lve::Renderer::getFrameIndex() const
{
	return currentFrameIndex;
}