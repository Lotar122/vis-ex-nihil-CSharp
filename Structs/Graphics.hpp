#pragma once

#include <iostream>

//this file stores all graphics related structures

//this is just used to mark that the variable is vulkan related
using vk_uint32_t = uint32_t;

namespace nihil::interop::graphics
{
	struct VertexAttribute {
		uint32_t binding;
		uint32_t location;

		vk_uint32_t format;
		uint32_t offset;
	};
	struct VertexBindingInformation {
		vk_uint32_t inputRate;
		uint32_t stride;
	};
}