#pragma once
#include <iostream>
#include <vector>
#include "Classes/Handle/Handle.hpp"
#include "Classes/VectorHandle/VectorHandle.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace nihil::graphics
{
	class Engine;
}

namespace nihil::interop::graphics
{
	class Engine_CS
	{
		nihil::graphics::Engine* engine = NULL;
	public:
		Engine_CS(nihil::graphics::Engine* _engine)
		{
			engine = _engine;
		}

		void Setup()
		{
			//engine->Setup();
		}

		interop::Handle CreatePipelineConfiguration()
		{

		}

		interop::Handle CreateShaderModule(MonoString* _filepath)
		{
			char* _filepathRaw = mono_string_to_utf8(_filepath);

			std::string filepath(_filepathRaw);
			//vk::ShaderModule* shaderModule = NULL;
			//engine->renderer->CreateShaderModule(filepath, engine->logicalDevice, &shaderModule);

			mono_free(_filepathRaw);

			interop::Handle handle = {};
			interop::EncodeHandle(&handle, /*shaderModule*/ NULL, "SHADEMOD");

			return handle;
		}
	};
}