#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "Classes/Handle/Handle.hpp"
#include "Classes/VectorHandle/VectorHandle.hpp"
#include "Structs/Example.hpp"

#include "Structs/Graphics.hpp"

#include "Classes/Memory/Types.hpp"

namespace nihil::interop
{
	union AnyHandle
	{
		Handle handle;
		VectorHandle handleVec;
	};

	class Engine_Mem
	{
	private:
		template<typename T>
		inline void deletion(void* ptr)
		{
			delete static_cast<T*>(ptr);
		}
		template<typename T>
		inline void vectorDeletion(void* ptr)
		{
			delete static_cast<std::vector<T>*>(ptr);
		}

		template<typename T>
		inline void deletion(uint64_t ptr)
		{
			delete reinterpret_cast<T*>(ptr);
		}
		template<typename T>
		inline void vectorDeletion(uint64_t ptr)
		{
			delete reinterpret_cast<std::vector<T>*>(ptr);
		}

		template<typename T>
		[[nodiscard]] inline void* allocation()
		{
			return new T();
		}
		template<typename T>
		[[nodiscard]] inline void* vectorAllocation()
		{
			return new std::vector<T>();
		}
	public:
		std::unordered_map<uint64_t, AnyHandle> handles = {};
		std::unordered_set<uint64_t> takenNames = {};
		std::unordered_set<uint64_t> reusableNames = {};
		uint64_t currentNameIndex = 0;

		uint64_t requestNewName()
		{
			if (reusableNames.size() == 0)
			{
				takenNames.insert(currentNameIndex);
				uint64_t res = currentNameIndex;
				currentNameIndex++;
				return res;
			}
			else
			{
				uint64_t res = *reusableNames.begin();
				reusableNames.erase(res);
				takenNames.insert(res);
				return res;
			}
		}

		void freeName(uint64_t name)
		{
			takenNames.erase(name);
			reusableNames.insert(name);
		}

		#pragma region Native Allocation
		//type is an encoded string, max of 8 characters
		interop::Handle allocateNativeObject(uint64_t _type)
		{
			Types type = (Types)_type;
			void* ptr = NULL;

			if (type == Types::INT)
			{
				ptr = allocation<int>();
			}
			else if (type == Types::INT16)
			{
				ptr = allocation<int16_t>();
			}
			else if (type == Types::INT32)
			{
				ptr = allocation<int32_t>();
			}
			else if (type == Types::INT64)
			{
				ptr = allocation<int64_t>();
			}

			else if (type == Types::UINT16)
			{
				ptr = allocation<uint16_t>();
			}
			else if (type == Types::UINT32)
			{
				ptr = allocation<uint32_t>();
			}
			else if (type == Types::UINT64)
			{
				ptr = allocation<uint64_t>();
			}

			else if (type == Types::CHAR)
			{
				ptr = allocation<char>();
			}
			else if (type == Types::BOOL)
			{
				ptr = allocation<bool>();
			}

			else if (type == Types::FLOAT)
			{
				ptr = allocation<float>();
			}
			else if (type == Types::DOUBLE)
			{
				ptr = allocation<double>();
			}

			else if (type == Types::Example)
			{
				ptr = allocation<nihil::interop::structs::Example>();
			}

			else if (type == Types::VertexAttribute)
			{
				ptr = allocation<nihil::interop::graphics::VertexAttribute>();
			}
			else if (type == Types::VertexBindingInformation)
			{
				ptr = allocation<nihil::interop::graphics::VertexBindingInformation>();
			}

			interop::Handle handle = {};

			EncodeHandle(&handle, ptr, _type);

			handle.id = requestNewName();

			AnyHandle handleAny = {};
			handleAny.handle = handle;

			handles.insert(std::make_pair(handle.id, handleAny));

			return handle;
		}

		//type is an encoded string, max of 8 characters
		void freeNativeObject(uint64_t ptr, uint64_t _type, uint64_t name)
		{
			Types type = (Types)_type;
			if (type == Types::INT)
			{
				deletion<int>(ptr);
			}
			else if (type == Types::INT16)
			{
				deletion<int16_t>(ptr);
			}
			else if (type == Types::INT32)
			{
				deletion<int32_t>(ptr);
			}
			else if (type == Types::INT64)
			{
				deletion<int64_t>(ptr);
			}

			else if (type == Types::UINT16)
			{
				deletion<uint16_t>(ptr);
			}
			else if (type == Types::UINT32)
			{
				deletion<uint32_t>(ptr);
			}
			else if (type == Types::UINT64)
			{
				deletion<uint64_t>(ptr);
			}

			else if (type == Types::CHAR)
			{
				deletion<char>(ptr);
			}
			else if (type == Types::BOOL)
			{
				deletion<bool>(ptr);
			}

			else if (type == Types::FLOAT)
			{
				deletion<float>(ptr);
			}
			else if (type == Types::DOUBLE)
			{
				deletion<double>(ptr);
			}

			else if (type == Types::Example)
			{
				deletion<nihil::interop::structs::Example>(ptr);
			}

			else if (type == Types::VertexAttribute)
			{
				deletion<nihil::interop::graphics::VertexAttribute>(ptr);
			}
			else if (type == Types::VertexBindingInformation)
			{
				deletion<nihil::interop::graphics::VertexBindingInformation>(ptr);
			}

			if (type == Types::NativeVector)
			{
				throw std::exception("Please use the freeNativeVector function to free native vectors, and input the element type as elementType");
			}
			else
			{
				handles.erase(name);
				freeName(name);
			}
		}

		interop::VectorHandle allocateNativeVector(uint64_t _elementType)
		{
			interop::VectorHandle handle = {};

			Types elementType = (Types)_elementType;

			void* ptr = NULL;

			Types type = elementType;

			if (type == Types::INT)
			{
				ptr = vectorAllocation<int>();
			}
			else if (type == Types::INT16)
			{
				ptr = vectorAllocation<int16_t>();
			}
			else if (type == Types::INT32)
			{
				ptr = vectorAllocation<int32_t>();
			}
			else if (type == Types::INT64)
			{
				ptr = vectorAllocation<int64_t>();
			}

			else if (type == Types::UINT16)
			{
				ptr = vectorAllocation<uint16_t>();
			}
			else if (type == Types::UINT32)
			{
				ptr = vectorAllocation<uint32_t>();
			}
			else if (type == Types::UINT64)
			{
				ptr = vectorAllocation<uint64_t>();
			}

			else if (type == Types::CHAR)
			{
				ptr = vectorAllocation<char>();
			}
			else if (type == Types::BOOL)
			{
				ptr = vectorAllocation<bool>();
			}

			else if (type == Types::FLOAT)
			{
				ptr = vectorAllocation<float>();
			}
			else if (type == Types::DOUBLE)
			{
				ptr = vectorAllocation<double>();
			}

			else if (type == Types::Example)
			{
				ptr = vectorAllocation<nihil::interop::structs::Example>();
			}

			else if (type == Types::VertexAttribute)
			{
				ptr = vectorAllocation<nihil::interop::graphics::VertexAttribute>();
			}
			else if (type == Types::VertexBindingInformation)
			{
				ptr = vectorAllocation<nihil::interop::graphics::VertexBindingInformation>();
			}

			interop::EncodeVectorHandle(&handle, ptr, _elementType);

			handle.id = requestNewName();

			AnyHandle handleAny = {};
			handleAny.handleVec = handle;

			handles.insert(std::make_pair(handle.id, handleAny));

			return handle;
		}

		void freeNativeVector(uint64_t ptr, uint64_t _elementType, uint64_t name)
		{
			Types elementType = (Types)_elementType;

			if (elementType == Types::VertexAttribute)
			{
				vectorDeletion<nihil::interop::graphics::VertexAttribute>(ptr);
			}
			else if (elementType == Types::VertexBindingInformation)
			{
				vectorDeletion<nihil::interop::graphics::VertexBindingInformation>(ptr);
			}

			else if (elementType == Types::INT)
			{
				vectorDeletion<int>(ptr);
			}
			else if (elementType == Types::INT16)
			{
				vectorDeletion<int16_t>(ptr);
			}
			else if (elementType == Types::INT32)
			{
				vectorDeletion<int32_t>(ptr);
			}
			else if (elementType == Types::INT64)
			{
				vectorDeletion<int64_t>(ptr);
			}

			else if (elementType == Types::UINT16)
			{
				vectorDeletion<uint16_t>(ptr);
			}
			else if (elementType == Types::UINT32)
			{
				vectorDeletion<uint32_t>(ptr);
			}
			else if (elementType == Types::UINT64)
			{
				vectorDeletion<uint64_t>(ptr);
			}

			else if (elementType == Types::BOOL)
			{
				vectorDeletion<bool>(ptr);
			}
			else if (elementType == Types::CHAR)
			{
				vectorDeletion<char>(ptr);
			}

			else if (elementType == Types::Example)
			{
				vectorDeletion<structs::Example>(ptr);
			}

			handles.erase(name);
			freeName(name);
		}
#pragma endregion

		#pragma region Script Allocation
		uint64_t allocateNativeObject_CS(uint64_t type)
		{
			interop::Handle handle = allocateNativeObject(type);
			return handle.id;
		}
		void freeNativeObject_CS(uint64_t name)
		{
			auto it = handles.find(name);
			uint64_t ptr = 0, type = 0;
			ptr = it->second.handle.ptr;
			type = it->second.handle.type;
			freeNativeObject(ptr, type, name);
		}

		uint64_t allocateNativeVector_CS(uint64_t elementType)
		{
			interop::VectorHandle handle = allocateNativeVector(elementType);
			return handle.id;
		}
		void freeNativeVector_CS(uint64_t name)
		{
			auto it = handles.find(name);
			uint64_t ptr = 0, type = 0;
			ptr = it->second.handleVec.ptr;
			type = it->second.handleVec.elementType;
			freeNativeVector(ptr, type, name);
		}
		#pragma endregion

		uint64_t getPtr(uint64_t name, bool isVector = false)
		{
			if (isVector)
			{
				return handles.find(name)->second.handleVec.ptr;
			}
			else
			{
				return handles.find(name)->second.handle.ptr;
			}
		}

		uint64_t getType(uint64_t name, bool isVector = false, bool elementType = true)
		{
			if (isVector)
			{
				if (elementType)
				{
					return handles.find(name)->second.handleVec.elementType;
				}
				else
				{
					return handles.find(name)->second.handleVec.vectorType;
				}
			}
			else
			{
				return handles.find(name)->second.handle.type;
			}
		}
	};
}