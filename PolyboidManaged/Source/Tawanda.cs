using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;


namespace Polyboid
{
    public class Tawanda
    {
        public float health = 100.0f;
        private string m_Name = "Tawanda";
        protected float powerUp = 2;
        internal int gameState = 0;

        public string Name
        {
            get => m_Name;
            set
            {
                m_Name = value;
                health -= 20;
            }
        }


        public void PrintHealth()
        {
           
            Console.WriteLine("Health is at = {0:F}", health);
        }

        public void PrintAll()
        {


            Console.WriteLine("Health {0} Name {1} PowerUp {2} GameState {3}", health, Name, powerUp, gameState);
        }

        public int SayName(int age, int money, String name)
        {
        
            Console.WriteLine("{0} has {1} ${2}", name, age, money);

            return 200;
        }

    }
}
