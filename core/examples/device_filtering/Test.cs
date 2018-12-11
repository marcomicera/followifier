using System;
using System.Collections.Generic;

namespace device_filtering
{
    class Test
    {
        private const int SIZE = 4;
        private const int PACKET_SIZE = 10;

        public static void Main(string[] args)
        {
            Random rnd = new Random();
            List<Packet>[] list = new List<Packet>[SIZE];
            string chars = "ABCDEFGHIJK";

            for (int i=0; i<list.Length; i++){
                list[i] = new List<Packet>();
            }

            for (int i=0; i<list.Length; i++) {
                for (int j = 0; j < PACKET_SIZE; j++) {
                    string hash;
                    do {
                        hash = Convert.ToString(chars[rnd.Next(0, chars.Length)]);
                    } while (list[i].Exists(x => x.getHash().Equals(hash)));
                    list[i].Add(new Packet("", "", DateTime.Now, hash, 0)) ;
                }
            }

            Console.WriteLine("Testing:");
            for (int i=0; i<list.Length; i++) {
                Console.WriteLine("\nFrom ESP32 n " + i);
                foreach (Packet packet in list[i]) {
                    Console.WriteLine(packet.ToString());
                }       
            }
            Filterer filterer = new Filterer(list);

            Console.WriteLine("\n Number of devices: " + filterer.numberDevices());
            Console.ReadKey();
        }
    }
}
