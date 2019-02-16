using System;
using System.Collections.Generic;

namespace device_location {
    class Device {
        private readonly string MAC;
        private Point p; //current location
        private Point[] list; //list of last locations
        private int current;

    public Device(string MAC) {
        this.MAC = MAC;
        this.p = null;
        this.list = new Point[Constants.TIME_SLOTS];
        this.current = 0;
    }

    public bool CalculateLocation(Board[] esp, double[] signalPowerPerBoard) {
        if (esp.Length == signalPowerPerBoard.Length && esp.Length > 1) {
            Point newP = new Point(0,0);

            List<Point> listPoints = new List<Point>();

            //algorithm can be changed - I used one I thougth to
            for (int i = 0; i < esp.Length - 1; i++) {
                    for (int j = i+1; j < esp.Length; j++) {
                        //calculate interception points
                        //d = sqrt((x1-x2)^2 + (y1-y2)^2)
                        double d = Math.Sqrt(Math.Pow((esp[i].GetX() - esp[j].GetX()), 2) +
                            Math.Pow((esp[i].GetY() - esp[j].GetY()), 2));
                        //l = r1^2-r2^2 + d^2 / 2d
                        double l = (Math.Pow(signalPowerPerBoard[i], 2) - Math.Pow(signalPowerPerBoard[j], 2) + Math.Pow(d, 2)) / (2 * d);

                        //h = sqrt(r1^2 - l^2)
                        double h = Math.Sqrt(Math.Pow(signalPowerPerBoard[i], 2) - Math.Pow(l, 2));

                        //x = l(x2 - x1) / d + -h(y2 - y1) / d + x1
                        double x = (l * (esp[j].GetX() - esp[i].GetX())) / d + (h * (esp[j].GetY() - esp[i].GetY())) / d + esp[i].GetX();
                        //y = h(y2 - y1) / d -+ l(x2 - x1) / d + y1
                        double y = (l * (esp[j].GetY() - esp[i].GetY())) / d - (h * (esp[j].GetX() - esp[i].GetX())) / d + esp[i].GetY();
                        

                        if (CheckPoint(Math.Round(x), Math.Round(y), esp, signalPowerPerBoard)) {
                            if (!listPoints.Contains(new Point(Math.Round(x), Math.Round(y))))
                                listPoints.Add(new Point(Math.Round(x), Math.Round(y)));
                        }

                        //x = l(x2 - x1) / d + -h(y2 - y1) / d + x1
                        x = (l * (esp[j].GetX() - esp[i].GetX())) / d - (h * (esp[j].GetY() - esp[i].GetY())) / d + esp[i].GetX();
                        //y = h(y2 - y1) / d -+ l(x2 - x1) / d + y1
                        y = (l * (esp[j].GetY() - esp[i].GetY())) / d + (h * (esp[j].GetX() - esp[i].GetX())) / d + esp[i].GetY();


                        if (CheckPoint(Math.Round(x), Math.Round(y), esp, signalPowerPerBoard)) {
                            if (!listPoints.Contains(new Point(Math.Round(x), Math.Round(y))))
                                listPoints.Add(new Point(Math.Round(x), Math.Round(y)));
                        }

                    }
                }

                //calculate center

                double Px = 0, Py = 0;
                foreach (Point p in listPoints) {
                    Px += p.GetX();
                    Py += p.GetY();
                }

                p = new Point(Px / listPoints.Count, Py / listPoints.Count);

                list[current] = p;
                current++;
                if (current == Constants.TIME_SLOTS)
                    current = 0;

                return true;
            } else {
                return false;
            }
        }

        private bool CheckPoint(double x, double y, Board[] esp, double[] signal) {
            for (int i = 0; i < esp.Length; i++) {
                    if ((Math.Pow(x - esp[i].GetX(), 2) + Math.Pow(y - esp[i].GetY(), 2) - Math.Pow(signal[i], 2)) > 0)
                    return false;
            }
            return true;
        }

        public string GetMAC() => MAC;
        public Point GetLocation() => p;
        public Point[] GetList() => list;
    }
}