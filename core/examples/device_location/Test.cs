using System;
using System.Collections.Generic;


namespace device_location {
    public class Test {
        public static void Main(string[] args) {
            Station[] listStation = new Station[Constants.SIZE];
            Device d = new Device("A");

            for (int i=0; i<Constants.TIME_SLOTS+1; i++) {
                listStation[0] = new Station(1, i, 0);
                listStation[1] = new Station(2, i+2, 0);
                listStation[2] = new Station(3, i+1, -1);

                double[] listDistances = new double[Constants.SIZE];
                listDistances[0] = 1;
                listDistances[1] = 1;
                listDistances[2] = 1;

                d.CalculateLocation(listStation, listDistances);
                Console.WriteLine("TIME SLOT: " + i);
                Console.WriteLine("CURRENT LOCATION: " + d.GetLocation());
                Console.WriteLine("SLOTS:");
                Point[] list = d.GetList();
                for(int t=0; t<Constants.TIME_SLOTS; t++) {
                    Console.WriteLine("SLOT " + t + ": " + list[t]);
                }

                Console.ReadKey();
            }

        }

    }
}