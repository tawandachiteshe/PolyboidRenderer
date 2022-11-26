using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Polyboid
{
    public abstract class GameObject
    {
        public ulong gameObjectID = 0;


        public void AddComponent<T>()
        {

        }

        public T GetComponent<T>() where T : Component, new()
        {
            T component = new T
            { 
                uuid = gameObjectID
            };

            return component;

        }


    }
}
