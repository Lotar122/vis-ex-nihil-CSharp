#pragma once

#include <string>

namespace nihil::interop
{
	struct Handle
	{
		uint64_t ptr;
		uint64_t type;

		uint64_t id;
	};

	//type has to be exactly 9 characters long, with the last one being NULL '\0'
	void EncodeHandle(Handle* handle, void* ptr, const char* type)
	{
		uint64_t encoded = 0;
		for (int i = 0; i < 8; i++)
		{
			encoded |= (static_cast<uint64_t>(type[i]) << (i * 8));
		}

		handle->type = encoded;
		handle->ptr = reinterpret_cast<uint64_t>(ptr);
	}

	//type is an already encoded string as a uint64_t
	void EncodeHandle(Handle* handle, void* ptr, uint64_t type)
	{
		handle->type = type;
		handle->ptr = reinterpret_cast<uint64_t>(ptr);
	}

	std::pair<void*, std::string> DecodeHandle(Handle* handle)
	{
		void* ptr = NULL;
		std::string type = "........\0";

		ptr = reinterpret_cast<void*>(handle->ptr);
		
		for (int i = 0; i < 8; i++)
		{
			type[i] = static_cast<char>((handle->type >> (i * 8)) & 0xFF);
		}

		type[8] = '\0';

		return std::make_pair(ptr, type);
	}

	void* GetHandlePointer(Handle* handle)
	{
		return reinterpret_cast<void*>(handle->ptr);
	}

	std::string GetHandleType(Handle* handle)
	{
		std::string type = "........";

		for (int i = 0; i < 8; i++)
		{
			type[i] = static_cast<char>((handle->type >> (i * 8)) & 0xFF);
		}

		type[8] = '\0';

		return type;
	}

	constexpr uint64_t charToUint(const char* str)
	{
		uint64_t encoded = 0;
		for (int i = 0; i < 8; i++)
		{
			encoded |= (static_cast<uint64_t>(str[i]) << (i * 8));
		}

		return encoded;
	}
}