#include "Context/Window.hpp"

namespace ig = iganasuk;

ig::Window::Window(int w, int h, std::string const& title_name)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	__ptr = glfwCreateWindow(w, h, title_name.c_str(), nullptr, nullptr);
	if (__ptr == nullptr)
		throw std::runtime_error("failed to create window");
}

ig::Window::~Window(void)
{
	glfwDestroyWindow(__ptr);
	glfwTerminate();
}