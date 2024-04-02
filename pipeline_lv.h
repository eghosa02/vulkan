#pragma once
#include <string>
#include <vector>
#include <ostream>
#include "engineDevice.h"
#include "model.h"
#include <fstream>
#include <ostream>
#include <iostream>


namespace lve {
	struct pipelineConfigInfo{
		pipelineConfigInfo(const pipelineConfigInfo&) = delete;
		pipelineConfigInfo& operator=(const pipelineConfigInfo&) = delete;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};


	class Pipeline_lv
	{
		private:
			std::string vertFilePath;
			std::string fragFilePath;
			EngineDevice& device;
			VkPipeline graficPipeline;
			VkShaderModule vertShaderModule;
			VkShaderModule fragShaderModule;

			/*static*/ std::vector<char> readFile(const std::string&);
			void createGraphicPipeline(const pipelineConfigInfo&);
			void createShaderModule(const std::vector<char>&, VkShaderModule*);

		public:
			Pipeline_lv(const std::string &, const std::string &, EngineDevice&, const pipelineConfigInfo&);
			Pipeline_lv(const Pipeline_lv&) = delete;
			Pipeline_lv& operator=(const Pipeline_lv&) = delete;
			~Pipeline_lv();
			static void defaultPipelineConfigInfo(pipelineConfigInfo&);
			void bind(const VkCommandBuffer&);
	};
}