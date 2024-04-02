#pragma once

#include "windows_lv.h"
#include "engineDevice.h"
#include "swap_chain.h"
#include <memory>
#include <vector>
#include <array>
#include <iostream>


namespace lve {

	class Renderer {

		private:
			Windows_lv& window;
			EngineDevice& device;
			std::unique_ptr<EngineSwapChain> swapChain;
			std::vector<VkCommandBuffer> commandBuffers;
			uint32_t currentImageIndex;
			int currentFrameIndex;
			bool isFrameStarted;

			void freeCommandBuffers();
			void createCommandBuffer();
			void recreateSwapChain();

		public:
			Renderer(Windows_lv&, EngineDevice&);
			~Renderer();
			VkCommandBuffer beginFrame();
			void endFrame();
			void beginSwapChainRenderPass(VkCommandBuffer);
			void endSwapChainRenderPass(VkCommandBuffer);
			bool isFrameInProgress() const;
			VkCommandBuffer getCurrentCommandBuffer() const;
			VkRenderPass getSwapChainRenderPass() const;
			float getAspectRation() const;
			int getFrameIndex() const;
	};
}