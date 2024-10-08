using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Script.Interfaces;
using Script.Native;
using Script.Native.Interop;

namespace Script
{
    public class Script : IScript
    {
        public Script() 
        {
            Handle handle1 = Memory.allocateNativeObject(Types.INT);
            Handle handle2 = Memory.allocateNativeObject(Types.INT);
            handle1.assign<int>(10);
            handle2.assign<int>(9);
            Console.WriteLine(Example.handleTest(handle1.name, handle2.name));
            handle1.free();
            handle2.free();
        }

        public void keyEvent(char key)
        {
            //used to tell the script that a key has been pressed
        }

        public void physicsTick()
        {
            //physics related stuff goes here (runs at every physics frame, so more or less 40 PFPS)
            Console.WriteLine("Physics Ticked");
        }

        public void graphicsTick()
        {
            //graphics related stuff goes here (runs at every frame)
            Console.WriteLine("Graphics Ticked");
        }
    }
}
