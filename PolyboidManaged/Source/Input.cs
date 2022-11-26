using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Polyboid
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCodes codes)
        {
            return Internals.Engine_InputPressed(ref codes);
        }
    }
}
