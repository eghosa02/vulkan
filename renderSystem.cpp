#include "renderSystem.h"

lve::FirstRenderSystem::FirstRenderSystem(EngineDevice& dev, VkRenderPass rendPass) : device(dev)
{
	createPipelineLayout();
	createPipeline(rendPass);
}

lve::FirstRenderSystem::~FirstRenderSystem()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void lve::FirstRenderSystem::createPipelineLayout()
{
	VkPushConstantRange pushConst{};
	pushConst.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConst.offset = 0;
	pushConst.size = sizeof(pushContentData);


	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConst;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}

void lve::FirstRenderSystem::createPipeline(VkRenderPass renderPass)
{
	lve::pipelineConfigInfo pipelineConfig{};
	lve::Pipeline_lv::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<lve::Pipeline_lv>("shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", device, pipelineConfig);
}


void lve::FirstRenderSystem::renderGameObject(VkCommandBuffer commandBuffer, std::vector<GameObject_lv>& gameObjects, const Camera& camera)
{
	pipeline->bind(commandBuffer);
	auto projectionView = camera.getProjection() * camera.getView();
	for (auto& obj : gameObjects)
	{
		pushContentData push{};
		push.color = obj.color;
		push.transform = projectionView * obj.transform.mat4();

		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushContentData), &push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}
