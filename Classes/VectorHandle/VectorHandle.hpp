#pragma once

#include <iostream>

#include "../Handle/Handle.hpp"

namespace nihil::interop
{
	struct VectorHandle
	{
		uint64_t ptr;
		uint64_t elementType;
		uint64_t vectorType = interop::charToUint("STDVEC  ");
		uint64_t id;
	};

	//elementType has to be a 9 character long string, terminated with a NULL '\0' character
	void EncodeVectorHandle(VectorHandle* handle, void* ptr, const char* elementType)
	{
		uint64_t encoded = 0;
		for (int i = 0; i < 8; i++)
		{
			encoded |= (static_cast<uint64_t>(elementType[i]) << (i * 8));
		}

		handle->elementType = encoded;
		handle->ptr = reinterpret_cast<uint64_t>(ptr);
	}

	//elementType is an already encoded string in a uint64_t
	void EncodeVectorHandle(VectorHandle* handle, void* ptr, uint64_t elementType)
	{
		handle->elementType = elementType;
		handle->ptr = reinterpret_cast<uint64_t>(ptr);
	}

	std::string strFromUint(uint64_t str)
	{
		std::string type = "........\0";

		for (int i = 0; i < 8; i++)
		{
			type[i] = static_cast<char>((str >> (i * 8)) & 0xFF);
		}

		type[8] = '\0';

		return type;
	}

	//the std::string is the element type information !!! remember that the vectors type can be converted to string with: strFromUint
	std::pair<void*, std::string> DecodeVectorHandle(VectorHandle* handle)
	{
		void* ptr = NULL;
		std::string type = "........\0";

		ptr = reinterpret_cast<void*>(handle->ptr);

		for (int i = 0; i < 8; i++)
		{
			type[i] = static_cast<char>((handle->elementType >> (i * 8)) & 0xFF);
		}

		type[8] = '\0';

		return std::make_pair(ptr, type);
	}
}