using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Polyboid
{

    public class Component
    {
        public ulong uuid = 0u;
    }

    public class Transform : Component
    {
      
        public Vector3 Position
        {
            get
            {
                Internals.Transform_GetPosition(ref uuid, out Vector3 result);
                return result;
            }
            set => Internals.Transform_SetPosition(ref uuid, ref value);
        }


    }
}
