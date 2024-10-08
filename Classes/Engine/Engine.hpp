#pragma once

#include <unordered_map>
#include <string>
#include <exception>

#include <sstream>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace nihil::scripting
{
	class Engine
	{
	private:
		MonoDomain* domain = NULL;
		MonoAssembly* assembly = NULL;
		MonoImage* image = NULL;
		MonoClass* ScriptMain = NULL;
		MonoMethodDesc* MainMethodDesc = NULL;
		MonoMethod* MainMethod = NULL;
		MonoObject* nullObj = NULL;
	public:
		MonoClass* IScript = NULL;
		MonoObject* ScriptObject = NULL;

		std::unordered_map<std::string, uint32_t> GCObjects = {};

		Engine()
		{
			domain = mono_jit_init("nihil-scripting");
			mono_set_dirs(".", ".");
			assembly = mono_domain_assembly_open(domain, "Script.dll");
			if (!assembly)
			{
				throw std::exception("Couldn't load assembly");
				return;
			}
			image = mono_assembly_get_image(assembly);
			if (!image)
			{
				throw std::exception("Failed to create image");
				return;
			}
			ScriptMain = mono_class_from_name(image, "Script", "ScriptMain");
			IScript = mono_class_from_name(image, "Script.Interfaces", "IScript");
			if ((!ScriptMain) || (!IScript))
			{
				throw std::exception("Failed to get classes, make sure that your Script.dll has: \n An interface: IScript, for class Script \n A class: Script, that's your script, make it extend IScript \n A class: ScriptMain, this classes main() returns a Script");
				return;
			}
			MainMethodDesc = mono_method_desc_new(".ScriptMain::main()", false);
			if (!MainMethodDesc)
			{
				throw std::exception("Failed to create a method descriptor");
				return;
			}
			MainMethod = mono_method_desc_search_in_class(MainMethodDesc, ScriptMain);
			if (!MainMethod)
			{
				throw std::exception("Failed to get the main() method");
				return;
			}
		}

		~Engine()
		{
			auto it = GCObjects.find("ScriptObject");
			mono_gchandle_free(it->second);

			GCObjects.erase("ScriptObject");

			mono_jit_cleanup(domain);
		}

		void Start()
		{
			ScriptObject = mono_runtime_invoke(MainMethod, NULL, NULL, &nullObj);
			if (!ScriptObject)
			{
				throw std::exception("There was an error getting the Script interface");
				return;
			}

			uint32_t ScriptObjectGC = mono_gchandle_new(ScriptObject, false);
			GCObjects.insert(std::make_pair("ScriptObject", ScriptObjectGC));
		}

		MonoObject* invokeScriptMethod(std::string name, MonoObject* instance, int numArgs, void** args)
		{
			MonoClass* objClass = mono_object_get_class(instance);
			MonoMethod* method = mono_class_get_method_from_name(objClass, name.c_str(), numArgs);

			MonoObject* exception = NULL;
			MonoObject* result = mono_runtime_invoke(method, instance, args, &exception);

			return result;
		}

		void addNativeCall(std::string classAndScope, std::string functionName, std::string functionSignature, const void* function, bool temp = false)
		{
			functionSignature.erase(std::remove_if(functionSignature.begin(), functionSignature.end(), ::isspace), functionSignature.end());
			std::string completeSig = classAndScope + "::" + functionName + functionSignature;
			mono_add_internal_call(completeSig.c_str(), function);
			if (temp)
			{
				std::vector<std::string> sVec = {};
				std::stringstream sstream(functionSignature);
				std::string line = "";
				while (std::getline(sstream, line, ','))
				{
					sVec.push_back(line);
				}
				for (std::string& sr : sVec)
				{
					sr.erase(remove(sr.begin(), sr.end(), '('), sr.end());
					sr.erase(remove(sr.begin(), sr.end(), ')'), sr.end());
				}
				std::string x = "[MethodImpl(MethodImplOptions.InternalCall)]\npublic static extern void " + functionName + "(" + sVec[0] + " val, " + sVec[1] + " name);";
				std::cout << x << std::endl;
			}
		}
	};
}