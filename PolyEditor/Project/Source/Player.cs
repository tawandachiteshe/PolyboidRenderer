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
        public float power;
        public String name;
        public Vector3 position;
        public bool isHaha;
        public int Hehe;
        public double ju;
       // public uint id;
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
            if (Input.IsKeyPressed(KeyCodes.A))
            {
                var transform = GetComponent<Transform>();
                dir.x -= 2.0f * dt;
                transform.Position = dir;
                Console.WriteLine("A pressed");
            }

            if ((Input.IsKeyPressed(KeyCodes.D)))
            {
                var transform = GetComponent<Transform>();
                dir.x += 2.0f * dt;
                transform.Position = dir;
            }

            if ((Input.IsKeyPressed(KeyCodes.W)))
            {
                var transform = GetComponent<Transform>();
                dir.y -= 2.0f * dt;
                transform.Position = dir;
            }

            if ((Input.IsKeyPressed(KeyCodes.S)))
            {
                var transform = GetComponent<Transform>();
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
