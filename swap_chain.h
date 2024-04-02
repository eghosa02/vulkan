#pragma once

#include "engineDevice.h"
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace lve {

    class EngineSwapChain {
        private:
            void init();
            void createSwapChain();
            void createImageViews();
            void createDepthResources();
            void createRenderPass();
            void createFramebuffers();
            void createSyncObjects();

            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
            VkFormat swapChainImageFormat;
            VkFormat swapChainFormat;
            VkExtent2D swapChainExtent;
            std::vector<VkFramebuffer> swapChainFramebuffers;
            VkRenderPass renderPass;
            std::vector<VkImage> depthImages;
            std::vector<VkDeviceMemory> depthImageMemorys;
            std::vector<VkImageView> depthImageViews;
            std::vector<VkImage> swapChainImages;
            std::vector<VkImageView> swapChainImageViews;
            EngineDevice& device;
            VkExtent2D windowExtent;
            VkSwapchainKHR swapChain;
            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;
            std::vector<VkFence> imagesInFlight;
            std::shared_ptr<EngineSwapChain> oldSwapChain;
            size_t currentFrame = 0;
        public:
            static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
            EngineSwapChain(EngineDevice&, VkExtent2D);
            EngineSwapChain(EngineDevice&, VkExtent2D, std::shared_ptr<EngineSwapChain>);
            ~EngineSwapChain();
            EngineSwapChain(const EngineSwapChain&) = delete;
            EngineSwapChain& operator=(const EngineSwapChain&) = delete;
            VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
            VkRenderPass getRenderPass() { return renderPass; }
            VkImageView getImageView(int index) { return swapChainImageViews[index]; }
            size_t imageCount() { return swapChainImages.size(); }
            VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
            VkExtent2D getSwapChainExtent() { return swapChainExtent; }
            uint32_t width() { return swapChainExtent.width; }
            uint32_t height() { return swapChainExtent.height; }
            float extentAspectRatio() {
                return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
            }
            VkFormat findDepthFormat();
            VkResult acquireNextImage(uint32_t* imageIndex);
            VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
            bool compareSwapFormats(const EngineSwapChain&) const;
    };
}

