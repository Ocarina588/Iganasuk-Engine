#pragma once

#include <iostream>
#include <vulkan/vulkan.h>

namespace iganasuk {

	class RenderPass {
	public:
		RenderPass(void);
		~RenderPass(void);

		void init(void);

		operator VkRenderPass(void) { return __ptr; }

	private:
		VkRenderPass __ptr;
	};
}