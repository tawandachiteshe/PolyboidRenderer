using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Polyboid;

namespace Polyboid
{
    public class Player : GameObject
    {
        public float power = 0;
        public String name;
        public Vector3 position;
        public bool usePosition;
        public int Hehe;
        public double ju;
        public uint id;
        public ulong hahadsahuid;

       private Vector3 dir;

        public void OnConstruct()
        {
          dir = new Vector3();
        }

        public  void OnBeginPlay()
        {
            Console.WriteLine("On PLayer play {0}", gameObjectID);
            Console.WriteLine("id {0}", gameObjectID);

             var transform = GetComponent<Transform>();
            
            Console.WriteLine(" x {0} y {1} z {2}",transform.Position.x, transform.Position.y, transform.Position.z);

        }

        public void OnUpdate(float dt)
        {

            // Console.WriteLine("Power {0}", power);

            dir.x = power;

            if (usePosition)
            {
                dir = position;
            }
           

            var transform = GetComponent<Transform>();
            transform.Position = dir;

            if (Input.IsKeyPressed(KeyCodes.A))
            {
           
                dir.x -= 2.0f * dt;
                transform.Position = dir;
                Console.WriteLine("A pressed");
            }

            if ((Input.IsKeyPressed(KeyCodes.D)))
            {
                
                dir.x += 2.0f * dt;
                transform.Position = dir;
            }

            if ((Input.IsKeyPressed(KeyCodes.W)))
            {
                dir.y -= 2.0f * dt;
                transform.Position = dir;
            }

            if ((Input.IsKeyPressed(KeyCodes.S)))
            {
                
                dir.y += 2.0f * dt;
                transform.Position = dir;
            }
        }

        public void OnEndPlay()
        {
            Console.WriteLine("On End PLayer play");
        }
    }
}
