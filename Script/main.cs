using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Script.Interfaces;
using Script.Native.Interop;

using System.Runtime.CompilerServices;
using Script.Native;

namespace Script
{
    public class ScriptMain
    {
        public static IScript main()
        {
            return new Script();
        }
    }
}
