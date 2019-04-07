using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Localizer {
    public partial class Localizer : Form {
        List<String> stations = new List<String>();
        Configuration configFile;

        public Localizer(Configuration configFile) {
            this.configFile = configFile;
            var settings = configFile.AppSettings.Settings;
            foreach(var key in settings.AllKeys) {
                if (key.Contains("station_")) {
                    stations.Add(settings[key].Value);
                }
            }
            foreach (var key in settings.AllKeys) {
                System.Diagnostics.Debug.WriteLine("Key: {0} Value: {1}", key, settings[key].Value);
            }
            InitializeComponent();
        }

        private void resetSettings_Click(object sender, EventArgs e) {
            ConfigurationScreen configurationDialog = new ConfigurationScreen(configFile);
            configurationDialog.ShowDialog();
            foreach (var key in configFile.AppSettings.Settings.AllKeys) {
                System.Diagnostics.Debug.WriteLine("Key: {0} Value: {1}", key, configFile.AppSettings.Settings[key].Value);
            }
        }
    }
}
