using System;

namespace device_filtering
{
    class Packet
    {
        string MAC;
        string SSD;
        DateTime time;
        string hash;
        double signal;

        public Packet(string MAC, string SSD, DateTime time, string hash, double signal)
        {
            this.MAC = MAC.ToUpper();
            this.SSD = SSD.ToUpper();
            this.time = time;
            this.hash = hash.ToUpper();
            this.signal = signal;
        }

        public string getMAC() => MAC;
        public string getSSD() => SSD;
        public DateTime getTime() => time;
        public string getHash() => hash;
        public double getSignal() => signal;

        public override string ToString()
        {
            return "MAC: " + MAC + " SSD: " + SSD + " time: " + time + " hash: " + hash + " signal: " + signal;
        }
    }
}
