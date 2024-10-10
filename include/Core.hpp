#pragma once

#include <array>
#include "Context.hpp"

namespace iganasuk {



	class Core {
	public:
		Core(int ac, char** av);
		~Core(void);

		int main(void);
		void draw_frame(void);
		void record_command_buffer(void);
		void update_mvp(int i);

        float gray = 0.6f;
        std::vector<Vertex> vertices = {
                   {{-0.5f, -0.5f, -0.5f,},  {gray, gray, gray}},
                   {{ 0.5f, -0.5f, -0.5f,},  {gray, gray, gray}},
                   {{ 0.5f,  0.5f, -0.5f,},  {1.0f, 1.0f, 1.f}},
                   {{ 0.5f,  0.5f, -0.5f,},  {1.0f, 1.0f, 1.f}},
                   {{-0.5f,  0.5f, -0.5f,},  {1.0f, 1.0f, 1.f}},
                   {{-0.5f, -0.5f, -0.5f,},  {gray, gray, gray}},

                   //{{-0.5f, -0.5f,  0.5f,},  {0.0f, 1.0f, 0.f}},
                   //{{ 0.5f, -0.5f,  0.5f,},  {.0f, 1.0f, 0.f}},
                   //{{ 0.5f,  0.5f,  0.5f,},  {.0f, 1.0f, 0.f}},
                   //{{ 0.5f,  0.5f,  0.5f,},  {.0f, 1.0f, 0.f}},
                   //{{-0.5f,  0.5f,  0.5f,},  {0.0f, 1.0f, 0.f}},
                   //{{-0.5f, -0.5f,  0.5f,},  {0.0f, 1.0f, 0.f}},

                   {{-0.5f,  0.5f,  0.5f,},  {.0f, 1.0f, 0.f}},
                   {{-0.5f,  0.5f, -0.5f,},  {.0f, 1.0f, 0.f}},
                   {{-0.5f, -0.5f, -0.5f,},  {0.0f, 1.0f, 0.f}},
                   {{-0.5f, -0.5f, -0.5f,},  {0.0f, 1.0f, 0.f}},
                   {{-0.5f, -0.5f,  0.5f,},  {0.0f, 1.0f, 0.f}},
                   {{-0.5f,  0.5f,  0.5f,},  {.0f, 1.0f, 0.f}},

                   {{ 0.5f,  0.5f,  0.5f,},  {1.0f, 0.0f, 0.f}},
                   {{ 0.5f,  0.5f, -0.5f,},  {1.0f, 0.0f, 0.f}},
                   {{ 0.5f, -0.5f, -0.5f,},  {1.0f, 0.0f, 0.f}},
                   {{ 0.5f, -0.5f, -0.5f,},  {1.0f, 0.0f, 0.f}},
                   {{ 0.5f, -0.5f,  0.5f,},  {1.0f, 0.0f, 0.f}},
                   {{ 0.5f,  0.5f,  0.5f,},  {1.0f, 0.0f, 0.f}},

                   {{-0.5f, -0.5f, -0.5f,},  {gray, gray, gray}},
                   {{ 0.5f, -0.5f, -0.5f,},  {gray, gray, gray}},
                   {{ 0.5f, -0.5f,  0.5f,},  {1.0f, 1.0f, 1.f}},
                   {{ 0.5f, -0.5f,  0.5f,},  {1.0f, 1.0f, 1.f}},
                   {{-0.5f, -0.5f,  0.5f,},  {1.0f, 1.0f, 1.f}},
                   {{-0.5f, -0.5f, -0.5f,},  {gray, gray, gray}},

                   {{-0.5f,  0.5f, -0.5f,},  {0.0f, 1.0f, 1.f}},
                   {{ 0.5f,  0.5f, -0.5f,},  {.0f, 1.0f, 1.f}},
                   {{ 0.5f,  0.5f,  0.5f,},  {.0f, 1.0f, 1.f}},
                   {{ 0.5f,  0.5f,  0.5f,},  {.0f, 1.0f, 1.f}},
                   {{-0.5f,  0.5f,  0.5f,},  {0.0f, 1.0f, 1.f}},
                   {{-0.5f,  0.5f, -0.5f,},  {0.0f, 1.0f, 1.f}}
        };

        EventManager event_manager;
        Camera camera;

		CommandPool::Reset command_pool{ ig_device.info.index_graphics };
		CommandBuffer command_buffer{ command_pool };

		Fence in_flight;
		Semaphore image_available, finished_rendering;

		shader::Vertex		v{ "shaders/vert.spv" };
		shader::Fragment	f{ "shaders/frag.spv" };

		descriptor::Pool descriptor_pool{ 2, 2 };

        Mesh* meshes[2] = {
            Mesh::load("models/bunny.obj"),
            Mesh::load("cube", vertices.data(), vertices.size(), sizeof(Vertex))
        };

        Model models[2] = {
            { meshes[0], descriptor_pool, 2},
            { meshes[1], descriptor_pool, 1}
        };

		pipeline::Graphics pipeline { 
			{v, f}, 
			{Vertex::get_description()},
			{models[0].descriptor_set}
		};
	};
}