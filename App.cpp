#include "App.h"


App::App(const unsigned int& width, const unsigned int& heigh, const std::string& name, const bool& cm) :
	window(width, heigh, name), device(window), compile(cm), renderer(window, device)
{
	compileShaders(compile);
	loadGameObjects();
}

App::~App()
{

}

void App::run()
{
	lve::FirstRenderSystem renderSys(device, renderer.getSwapChainRenderPass());
    lve::Camera camera;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto gameObject = lve::GameObject_lv::createGameObject();
    lve::Controller controller;

	while (!window.shouldClose()) {
		glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;
        controller.move(window.getWindow(), frameTime, gameObject);
        camera.setViewYXZ(gameObject.transform.translation, gameObject.transform.rotation);

        float aspect = renderer.getAspectRation();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
		if (auto commandBuffer = renderer.beginFrame())
		{
			renderer.beginSwapChainRenderPass(commandBuffer);
			renderSys.renderGameObject(commandBuffer, gameObjects, camera);
			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}
	vkDeviceWaitIdle(device.device());
}


std::unique_ptr<lve::Model_lve> createCubeModel(lve::EngineDevice& device, glm::vec3 offset) {
    std::vector<lve::Model_lve::Vertex> vertices{

        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}}

    };
    for (auto& v : vertices) {
        v.position += offset;
    }
    return std::make_unique<lve::Model_lve>(device, vertices);
}


void App::loadGameObjects()
{
    std::shared_ptr<lve::Model_lve> model = createCubeModel(device, { .0f, .0f, .0f });

    auto mesh = lve::GameObject_lv::createGameObject();
    mesh.model = model;
    mesh.transform.translation = { .0f, .0f, 2.5f };
    mesh.transform.scale = { .5f, .5f, .5f };
    gameObjects.push_back(std::move(mesh));
}

void App::compileShaders(const bool& compile)
{
	if (compile) {
		int exitCode = std::system(".\\compile.bat");
		if (exitCode)
			throw std::runtime_error("failed to compile shaders");
	}
}