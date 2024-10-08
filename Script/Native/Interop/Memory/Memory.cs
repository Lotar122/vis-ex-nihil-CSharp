using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.CompilerServices;

namespace Script.Native.Interop
{
    public enum Types : ulong
    {
        INT = 2314885530821873225,
        INT16 = 2314885625596366409,
        INT32 = 2314885608450051657,
        INT64 = 2314885617090317897,

        UINT16 = 2314909793964149077,
        UINT32 = 2314905404507572565,
        UINT64 = 2314907616415730005,

        FLOAT = 2314885754713492550,
        DOUBLE = 2314926402301153092,

        BOOL = 2314885531559743298,
        CHAR = 2314885531659487299,

        Example = 2325348690383951941
    }

    public enum HandleTypes
    {
        Vector,
        Generic
    }

    public static class Memory
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong allocateNativeObject_CXX(ulong type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong freeNativeObject_CXX(ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong allocateNativeVector_CXX(ulong elementType);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong freeNativeVector_CXX(ulong name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong getPtr_CXX(ulong name, bool isVector = false);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong getType_CXX(ulong name, bool isVector = false, bool elementType = true);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_INT_CXX(int val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_INT16_CXX(short val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_INT32_CXX(int val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_INT64_CXX(long val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_UINT16_CXX(ushort val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_UINT32_CXX(uint val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_UINT64_CXX(ulong val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_FLOAT_CXX(float val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_DOUBLE_CXX(double val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_BOOL_CXX(bool val, ulong name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_CHAR_CXX(byte val, ulong name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void assign_Example_CXX(Structs.Example val, ulong name);

        public static ulong getType(ulong name, HandleTypes handleType, bool isElementType = true)
        {
            if(handleType == HandleTypes.Vector)
            {
                return getType_CXX(name, true, isElementType);
            }
            else if(handleType == HandleTypes.Generic)
            {
                return getType_CXX(name);
            }
            else
            {
                return getType_CXX(name);
            }
        }
        public static ulong getPtr(ulong name, HandleTypes handleType)
        {
            if(handleType == HandleTypes.Vector)
            {
                return getPtr_CXX(name, true);
            }
            else if(handleType == HandleTypes.Generic)
            {
                return getPtr_CXX(name);
            }
            else
            {
                return getPtr_CXX(name);
            }
        }

        public static Handle allocateNativeObject(Types type)
        {
            return new Handle(allocateNativeObject_CXX((ulong)type));
        }

        public static void assignHandle<T>(T val, Handle handle) where T : struct
        {
            if(typeof(T) == typeof(int))
            {
                assign_INT_CXX((int)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(short))
            {
                //assign_INT16_CXX((short)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(Int32))
            {
                assign_INT32_CXX((Int32)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(long))
            {
                assign_INT64_CXX((long)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(ushort))
            {
                //assign_UINT16_CXX((ushort)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(uint))
            {
                assign_UINT32_CXX((uint)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(ulong))
            {
                assign_UINT64_CXX((ulong)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(float))
            {
                assign_FLOAT_CXX((float)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(double))
            {
                assign_DOUBLE_CXX((double)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(bool))
            {
                assign_BOOL_CXX((bool)(object)val, handle.name);
            }
            else if(typeof(T) == typeof(char))
            {
                assign_CHAR_CXX(Convert.ToByte((char)(object)val), handle.name);
            }
            else if(typeof(T) == typeof(Structs.Example))
            {
                assign_Example_CXX((Structs.Example)(object)val, handle.name);
            }
        }
    }
}
