using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.CompilerServices;

namespace Script.Native.Interop
{
    public class Handle
    {
        public UInt64 ptr = 0;
        public UInt64 type = 0;

        public UInt64 name = 0;

        public static HandleTypes handleType = HandleTypes.Generic;

        public Handle(ulong _name)
        {
            name = _name;
            ptr = Memory.getPtr(name, HandleTypes.Generic);
            type = Memory.getType(name, HandleTypes.Generic);
        }

        public void assign<T>(T val) where T : struct
        {
            Memory.assignHandle<T>(val, this);
        }

        public void free()
        {
            Memory.freeNativeObject_CXX(name);
        }
    }

    public class VectorHandle
    {
        public UInt64 ptr = 0;
        public UInt64 elementType = 0;

        public UInt64 name = 0;

        public static HandleTypes handleType = HandleTypes.Generic;
        public static UInt64 vectorType = 2314924173547557971;

        public VectorHandle(ulong _name)
        {
            name = _name;
            ptr = Memory.getPtr(name, HandleTypes.Vector);
            elementType = Memory.getType(name, HandleTypes.Vector, true);
        }
    }
}
