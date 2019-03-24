using System;

namespace device_location {
    public class Board {
        readonly int id;
        double x;
        double y;

        public Board(int id, double x, double y) {
            this.id = id;
            this.x = x;
            this.y = y;
        }

        public void SetPosition(double x, double y) {
            this.x = x;
            this.y = y;
        }

        public int GetId() => id;
        public double GetX() => x;
        public double GetY() => y;
    }
}