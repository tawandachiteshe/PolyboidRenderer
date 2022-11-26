using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Polyboid
{
    public class Internals
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Transform_SetPosition(ref ulong entityID, ref Vector3 vector);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Transform_GetPosition(ref ulong entityID, out Vector3 vector);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool Engine_InputPressed(ref KeyCodes codes);
    }
}
