using System;
using System.Collections.Generic;

namespace device_filtering
{
    class Test
    {
        private static int size = 4;

        public static void Main(string[] args)
        {
            Random rnd = new Random();
            List<Packet>[] list = new List<Packet>[size];
            string chars = "ABCDEFGHIJK";

            for (int i=0; i<size; i++){
                list[i] = new List<Packet>();
            }

            for (int i=0; i<size; i++) {
                int packet_size = 10;
                for (int j = 0; j < packet_size; j++) {
                    string MAC;
                    do {
                        MAC = Convert.ToString(chars[rnd.Next(0, chars.Length)]);
                    } while (list[i].Exists(x => x.getMAC().Equals(MAC.ToUpper())));
                    list[i].Add(new Packet(MAC, "", DateTime.Now, "", 0)) ;
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
