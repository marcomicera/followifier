using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LiveCharts;
using LiveCharts.Defaults;
using System.Windows;

namespace Plot
{
    public partial class ScatterExample : Window
    {

        public ChartValues<ObservablePoint> ValuesStation { get; set; }
        public ChartValues<ObservablePoint> ValuesDevice { get; set; }

        public ScatterExample()
        {
            var r = new Random();
            ValuesStation = new ChartValues<ObservablePoint>();
            ValuesDevice = new ChartValues<ObservablePoint>();
            DataContext = this;
        }

        private void DefinePositionStation(int i, ObservablePoint P)
        {
            ValuesStation[i].X = P.X;
            ValuesStation[i].Y = P.Y;       
        }

        private void DefinePositionDevice(int i, ObservablePoint P)
        {
            ValuesDevice[i].X = P.X;
            ValuesDevice[i].Y = P.Y;
        }

        public void DefinitionStations(int N, List <ObservablePoint> P)
        {
            for (var i = 0; i < N; i++)
            {
                ValuesStation.Add(P.ElementAt(i));
            }
        }



        public int AddDevice(ObservablePoint P)
        {
            ValuesDevice.Add(P);
            return ValuesDevice.Count;
        }

    }
}
