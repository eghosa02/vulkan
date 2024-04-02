#pragma once

#include "renderer.h"
#include "renderSystem.h"
#include "camera.h"
#include <chrono>
#include "controller.h"


class App {
	private:
		lve::Windows_lv window;
		lve::EngineDevice device;
		lve::Renderer renderer;
		std::vector<lve::GameObject_lv> gameObjects;
		bool compile;

		void compileShaders(const bool&);
		void loadGameObjects();

	public:
		App(const unsigned int&, const unsigned int&, const std::string &, const bool&);
		App(const App&) = delete;
		App& operator=(const App&) = delete;
		~App();		
		void run();
};
