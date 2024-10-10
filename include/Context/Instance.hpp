#pragma once

#include <iostream>
#include <vulkan/vulkan.h>
#include <vector>


namespace iganasuk {

	class Instance {
	public:
		Instance(void);
		~Instance(void);

		void add_layer(char const* layer);
		void add_extension(char const* extension);

		void init(void);

		operator VkInstance (void) { return __ptr; }
	private:
		std::vector<char const*> __layers;
		std::vector<char const*> __extensions;

		VkDebugUtilsMessengerCreateInfoEXT get_messenger_create_info(void) const;
		void create_messenger(void);
		void destroy_messenger(void);

		VkInstance __ptr = nullptr;
		VkDebugUtilsMessengerEXT __messenger = nullptr;
	};
}

