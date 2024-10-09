#pragma once 

#include <iostream>
#include <GLFW/glfw3.h>

namespace iganasuk {

	class Window {
	public:
		Window(int w, int h, std::string const& name);
		~Window(void);
	private:
	};
}