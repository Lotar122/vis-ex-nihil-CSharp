using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Script.Interfaces
{
    public interface IScript
    {
        void physicsTick();

        void graphicsTick();

        void keyEvent(char key);
    }
}
