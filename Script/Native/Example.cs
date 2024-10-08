using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.CompilerServices;

namespace Script.Native
{
    public class Example
    {
        //adds two numbers
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern int add(int x, int y);
        //both of the handles have to hold a natively allocated int
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern int handleTest(ulong name1, ulong name2);
    }
}
