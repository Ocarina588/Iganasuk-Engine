#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace iganasuk {

	class EventManager {
	public:
		EventManager(void);
		~EventManager(void);

		void process_key(int key, int scancode, int action, int mods);
		void process_mouse(int button, int action, int mods);
		void process_scroll(double xoffset, double yoffset);
		glm::dvec2 mouse_offset;
		glm::dvec2 mouse_scroll;

		glm::dvec2 pos;
		glm::dvec2 last_pos;

		bool mouse_clicked = false;
		bool W = false, A = false, S = false, D = false, Q = false, E = false;
		bool u = false, d = false, r = false, l = false;
	};
}