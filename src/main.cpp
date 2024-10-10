#include <iostream>
#include "Core.hpp"

namespace ig = iganasuk;

int main(int ac, char** av)
{
	try {
		new ig::Context(1280, 720, "test");
		ig::Core core(ac, av);

		return core.main();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	return (0);
}

