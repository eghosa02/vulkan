#pragma once

#include "engineDevice.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include<glm/glm.hpp>
#include <vector>
#include <iostream>


namespace lve {

	class Model_lve {

		public:
			struct Vertex {
				glm::vec3 position;
				glm::vec3 color;
				static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			};
			Model_lve(EngineDevice&, const std::vector<Vertex>&);
			~Model_lve();
			Model_lve(const Model_lve&) = delete;
			Model_lve& operator=(const Model_lve&) = delete;
			void bind(VkCommandBuffer);
			void draw(VkCommandBuffer);

		private:
			EngineDevice& device;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			uint32_t vertexCount;
			void createVertexBuffers(const std::vector<Vertex>& vertices);
		};
}