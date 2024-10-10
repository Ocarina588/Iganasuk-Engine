#include "Context/Surface.hpp"

namespace ig = iganasuk;

ig::Surface::Surface(void)
{

}

ig::Surface::~Surface(void)
{
	vkDestroySurfaceKHR(__instance, __ptr, nullptr);
}

void ig::Surface::init(VkInstance instance, GLFWwindow *window)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &__ptr) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
	__instance = instance;
}