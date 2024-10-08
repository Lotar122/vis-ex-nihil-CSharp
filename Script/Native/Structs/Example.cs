using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace Script.Native.Structs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Example
    {
        public ulong vkHandle;
        public bool x;
    }
}
