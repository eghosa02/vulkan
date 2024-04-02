#pragma once

#include"model.h"
#include "memory"
#include <glm/gtc/matrix_transform.hpp>

namespace lve {

	struct Transform{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4(1.f), translation);
			transform = glm::rotate(transform, rotation.x, { 1.f, 0.f, 0.f });
			transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f });
			transform = glm::rotate(transform, rotation.z, { 0.f, 0.f, 1.f });
			transform = glm::scale(transform, scale);
			return transform;
		}
	};

	class GameObject_lv {
	public:
		std::shared_ptr<Model_lve> model{};
		glm::vec3 color{};
		Transform transform{};
		unsigned int id;
		static GameObject_lv createGameObject();
		unsigned int getId() const;
	private:
		GameObject_lv(const unsigned int&);
	};
}