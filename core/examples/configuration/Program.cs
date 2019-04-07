using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;

namespace Localizer {
    static class Program {
        static void Main() {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            ReadAllSettings();
            Application.Run(new Localizer(ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None)));
        }

        static void ReadAllSettings() {
            try {
                var appSettings = ConfigurationManager.AppSettings;

                if (appSettings.Count == 0) {
                    //Set configuration
                    try {
                        var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                        Application.Run(new ConfigurationScreen(configFile));
                        ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);
                    }
                    catch (ConfigurationErrorsException) {
                        Console.WriteLine("Error writing app settings");
                    }
                }
            }
            catch (ConfigurationErrorsException) {
                //error reading app configuration. Requesting configurations again
            }
        }
    }
}