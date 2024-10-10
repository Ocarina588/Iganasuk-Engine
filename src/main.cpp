#include <iostream>
#include "Context.hpp"
#include <set>

namespace ig = iganasuk;

int main(int ac, char** av)
{
	new ig::Context(1280, 720, "Vulkan app");

	while (ig_window.is_open())
		ig_window.poll_events();

	return (0);
}

