#include "model.h"

lve::Model_lve::Model_lve(EngineDevice& dev, const std::vector<Vertex>& vert): device(dev)
{
	createVertexBuffers(vert);
}

lve::Model_lve::~Model_lve()
{
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void lve::Model_lve::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
	VkBufferUsageFlags bufferFlag{};
	VkMemoryPropertyFlags memoryFlag{};
	device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);
	void* data;
	vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.device(), vertexBufferMemory);
}

void lve::Model_lve::draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void lve::Model_lve::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription> lve::Model_lve::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescr(1);
	bindingDescr[0].binding = 0;
	bindingDescr[0].stride = sizeof(Vertex);
	bindingDescr[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescr;
}

std::vector<VkVertexInputAttributeDescription> lve::Model_lve::Vertex::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescr(2);
	attributeDescr[0].binding = 0;
	attributeDescr[0].location = 0;
	attributeDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescr[0].offset = offsetof(Vertex, position);

	attributeDescr[1].binding = 0;
	attributeDescr[1].location = 1;
	attributeDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescr[1].offset = offsetof(Vertex, color);
	return attributeDescr;
}
