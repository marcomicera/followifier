using System.Collections.Generic;

namespace device_filtering
{
    class Filterer {
        List<Packet>[] packets;

        public Filterer(params List<Packet>[] packets){
            for (int i = 0; i < packets.Length; i++){
                this.packets = packets;
            }
        }

        //check if device is in room
        public bool checkDevice(string MAC){
            foreach (List<Packet> packets in packets)
            {
                if (!packets.Exists(p => p.getMAC().Equals(MAC.ToUpper())))
                    return false;
            }
            return true;
        }

        //returns number of devices
        public long numberDevices()
        {
            long n = 0;
            foreach(Packet packet in packets[0])
            {
                if (checkDevice(packet.getMAC()))
                    n++;
            }
            return n;
        }
    }
}
