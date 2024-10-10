#include "Core.hpp"
#include <glm/glm.hpp>

namespace ig = iganasuk;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ig::Core* core = (ig::Core *)glfwGetWindowUserPointer(ig_window);
	core->event_manager.process_key(key, scancode, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ig::Core* core = (ig::Core*)glfwGetWindowUserPointer(ig_window);
	core->event_manager.process_scroll(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ig::Core* core = (ig::Core*)glfwGetWindowUserPointer(ig_window);
	core->event_manager.process_mouse(button, action, mods);
}


ig::Core::Core(int ac, char** av)
{
	glfwSetWindowUserPointer(ig_window, this);
	glfwSetKeyCallback(ig_window, key_callback);
	glfwSetScrollCallback(ig_window, scroll_callback);
	glfwSetMouseButtonCallback(ig_window, mouse_button_callback);
}

ig::Core::~Core(void)
{

}

int ig::Core::main(void)
{
	while (ig_window.is_open()) {
		ig_window.poll_events();
		camera.process_input(event_manager);
		Context::update_dt();;;
		draw_frame();;
	};
	in_flight.wait();
	return (0);
}

void ig::Core::draw_frame(void)
{
	in_flight.wait(true);
	ig_swapchain.acquire_next_image(image_available);
	record_command_buffer();

	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo submit_info{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = image_available;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = finished_rendering;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = command_buffer;

	ig_device.submit(ig_device.queue_graphics, 1, &submit_info, in_flight);

	VkPresentInfoKHR present_info{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = finished_rendering;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = ig_swapchain;
	present_info.pImageIndices = &ig_swapchain.image_index;

	ig_device.present(ig_device.queue_present, present_info);
}

void ig::Core::record_command_buffer(void)
{
	command_buffer.begin();

	VkClearValue clear_values[] = { {{{0.0f, 0.0f, 0.0f, 1.0f}}}, {.depthStencil = {1.f, 0} } };
	VkRenderPassBeginInfo bi{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	bi.renderPass = ig_render_pass;
	bi.framebuffer = ig_swapchain.get_actual_framebuffer();
	bi.renderArea = { {}, ig_swapchain.extent };
	bi.clearValueCount = 2;
	bi.pClearValues = clear_values;

	command_buffer.begin_render_pass(bi);
	command_buffer.bind_pipeline_graphics(pipeline);;

	for (int i = 0; i < 2; i++) {
		if (i)
		update_mvp(i);
		command_buffer.bind_descriptior_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, 0, 1, models[i].descriptor_set);
		models[i].draw(command_buffer);
	}

	command_buffer.end_render_pass();
	command_buffer.end();
}

void ig::Core::update_mvp(int i)
{
	MVP ubo{};

	ubo.model = models[i].__model;;
	ubo.view = camera.view;
	ubo.proj = glm::perspective(glm::radians(45.0f), ig_swapchain.extent.width / (float)ig_swapchain.extent.height, 0.1f, 10000.0f);
	ubo.proj[1][1] *= -1;

	models[i].ubuffer.memcpy(&ubo);
}