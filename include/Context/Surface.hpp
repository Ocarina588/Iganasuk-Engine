#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace iganasuk {

	class Surface {
	public:
		Surface(void);
		~Surface(void);
		void init(VkInstance instance, GLFWwindow* window);
		operator VkSurfaceKHR(void) { return __ptr; };
	private:
		VkSurfaceKHR __ptr = nullptr;
		VkInstance __instance = nullptr;
	};
}