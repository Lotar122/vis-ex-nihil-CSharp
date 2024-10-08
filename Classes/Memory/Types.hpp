#pragma once

#include "Classes/Handle/Handle.hpp"

namespace nihil::interop
{
	enum class Types : uint64_t
	{
        INT = charToUint("INT     "),
        INT16 = charToUint("INT16   "),
        INT32 = charToUint("INT32   "),
        INT64 = charToUint("INT64   "),

        UINT16 = charToUint("UINT16  "),
        UINT32 = charToUint("UINT32  "),
        UINT64 = charToUint("UINT64  "),

        FLOAT = charToUint("FLOAT   "),
        DOUBLE = charToUint("DOUBLE  "),

        BOOL = charToUint("BOOL    "),
        CHAR = charToUint("CHAR    "),

        Example = charToUint("EXAMPLE "),

        NativeVector = charToUint("STDVEC  "),

        VertexAttribute = charToUint("VSATTRIB"),
        VertexBindingInformation = charToUint("VSBINDIF")
	};
}