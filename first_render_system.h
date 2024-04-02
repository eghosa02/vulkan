#pragma once

#include "pipeline_lv.h"
#include "engineDevice.h"
#include "gameObject.h"
#include <memory>
#include <vector>

#include <array>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include<glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {
	struct pushContentData
	{
		glm::mat2 transform{ 1.f };
		alignas(8) glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};
	class FirstRenderSystem {

		private:
			EngineDevice& device;
			std::unique_ptr<lve::Pipeline_lv> pipeline;
			VkPipelineLayout pipelineLayout;

			void createPipelineLayout();
			void createPipeline(VkRenderPass);

		public:
			FirstRenderSystem(EngineDevice&, VkRenderPass);
			~FirstRenderSystem();
			FirstRenderSystem(const FirstRenderSystem&) = delete;
			FirstRenderSystem& operator=(const FirstRenderSystem&) = delete;
			void renderGameObject(VkCommandBuffer, std::vector<GameObject_lv>&);
	};

}