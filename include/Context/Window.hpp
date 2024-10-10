#pragma once 

#include <iostream>
#include <GLFW/glfw3.h>

namespace iganasuk {

	class Window {
	public:
		Window(int w, int h, std::string const& name);
		~Window(void);
		operator GLFWwindow* (void) { return __ptr; };
		bool is_open(void) { return !glfwWindowShouldClose(__ptr); }
		void poll_events(void) { glfwPollEvents(); }
	private:
		GLFWwindow* __ptr = nullptr;
	};
}