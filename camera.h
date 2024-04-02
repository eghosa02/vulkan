#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include<glm/glm.hpp>

namespace lve {
	class Camera {
		private:
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
		public:
			Camera();
			void setOrtographicProjection(float, float, float, float, float, float);
			void setPerspectiveProjection(float, float, float, float);
			void setViewDirection(glm::vec3, glm::vec3, glm::vec3 = {0.f, -1.f, 0.f});
			void setViewTarget(glm::vec3, glm::vec3, glm::vec3 = { 0.f, -1.f, 0.f });
			void setViewYXZ(glm::vec3, glm::vec3);
			const glm::mat4& getProjection() const;
			const glm::mat4& getView() const;
	};
}