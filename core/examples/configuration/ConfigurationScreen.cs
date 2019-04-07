using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Localizer {
    public partial class ConfigurationScreen : Form {
        Configuration configFile;

        public ConfigurationScreen(Configuration configFile) {
            this.configFile = configFile;
            InitializeComponent();
            Label counterLabel = new Label();
            counterLabel.Text = "Number of stations";
            counterLabel.AutoSize = true;

            NumericUpDown counterValue = new NumericUpDown();
            counterValue.Name = "numberStationValue";
            counterValue.Minimum = 0;
            counterValue.ValueChanged += new System.EventHandler(this.NumberDevices_ValueChanged);
            
            tableLayoutPanel1.SetCellPosition(counterLabel, new TableLayoutPanelCellPosition(0, 0));
            tableLayoutPanel1.SetCellPosition(counterValue, new TableLayoutPanelCellPosition(1, 0));
            tableLayoutPanel1.Controls.Add(counterLabel);
            tableLayoutPanel1.Controls.Add(counterValue);
        }

        private void ConfigurationScreen_Load(object sender, EventArgs e) {

        }
        

        private void NumberDevices_ValueChanged(object sender, EventArgs e) {
            NumericUpDown np = (NumericUpDown) tableLayoutPanel1.GetControlFromPosition(1,0);
            int n = (int)np.Value;
            int prev = (tableLayoutPanel1.RowCount-1)/3;
            tableLayoutPanel1.RowCount = (int) (3 * n) + 1;

            if (prev < n)
                for (int i = prev + 1; i <= n; i++) {
                    NewItem(i);
                }
            else {
                tableLayoutPanel1.AutoScroll = false;
                tableLayoutPanel1.VerticalScroll.Visible = false;
                tableLayoutPanel1.AutoScroll = true;
                for (int i = prev-1; i >=n; i--) {
                    for(int j=3; j>0; j--) {
                        tableLayoutPanel1.RowStyles.RemoveAt(i*3 + j);
                        tableLayoutPanel1.Controls.Remove(tableLayoutPanel1.GetControlFromPosition(0, i * 3 + j));
                        tableLayoutPanel1.Controls.Remove(tableLayoutPanel1.GetControlFromPosition(1, i * 3 + j));
                    }
                }
            }
        }

        private void NewItem(int i) {
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            Label xLabel = new Label();
            xLabel.Text = "X: ";

            NumericUpDown numX = new NumericUpDown();
            numX.Name = "numX";

            Label yLabel = new Label();
            yLabel.Text = "Y : ";

            NumericUpDown numY = new NumericUpDown();
            numY.Name = "numY";

            Label label = new Label();
            label.Text = "Station " + i;

            int n = (i-1) * 3;

            tableLayoutPanel1.SetRow(label, n+1);
            tableLayoutPanel1.SetColumn(label, 0);
            tableLayoutPanel1.SetRow(xLabel, n+2);
            tableLayoutPanel1.SetColumn(xLabel, 0);
            tableLayoutPanel1.SetRow(numX, n + 2);
            tableLayoutPanel1.SetColumn(numX, 1);
            tableLayoutPanel1.SetRow(yLabel, n+3);
            tableLayoutPanel1.SetColumn(yLabel, 0);
            tableLayoutPanel1.SetRow(numY, n + 3);
            tableLayoutPanel1.SetColumn(numY, 1);

            tableLayoutPanel1.Controls.Add(label);
            tableLayoutPanel1.Controls.Add(xLabel);
            tableLayoutPanel1.Controls.Add(yLabel);
            tableLayoutPanel1.Controls.Add(numX);
            tableLayoutPanel1.Controls.Add(numY);
 
        }

        private void confirm_Click(object sender, EventArgs e) {
            var settings = configFile.AppSettings.Settings; 
            foreach(var key in settings.AllKeys) {
                settings.Remove(key);
            }
            NumericUpDown np = (NumericUpDown)tableLayoutPanel1.GetControlFromPosition(1, 0);
            int n = (int)np.Value;
            settings.Add("number_devices", n.ToString());
            for(int i=1; i<=n; i++) {
                settings.Add("station_"+i, ((NumericUpDown)tableLayoutPanel1.GetControlFromPosition(1, (i-1)*3+2)).Value.ToString() + " " + ((NumericUpDown)tableLayoutPanel1.GetControlFromPosition(1,(i - 1) * 3 + 3)).Value.ToString());
            }
            configFile.Save(ConfigurationSaveMode.Modified);
            Close();
        }
    }
}
