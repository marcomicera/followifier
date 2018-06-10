using System;
using System.Data.SQLite;
using System.Windows;

namespace Database
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            SQLiteConnection.CreateFile("MyDatabase.sqlite");
            SQLiteConnection conn;
            conn = new SQLiteConnection("Data Source=MyDatabase.sqlite;Version=3;");
            CreateTable(conn);
            FillDatabase(conn);
            FindPacketsByMAC(conn, "a");
            DeleteByTimestamp(conn, "2018-04-28 08:23:19.120");
            FindPacketsByMAC(conn, "a");
        }

        private void DeleteByTimestamp(SQLiteConnection conn, string time)
        {
            conn.Open();
            String sql = "DELETE FROM Data WHERE time <= '" + time + "';";
            SQLiteCommand command = new SQLiteCommand(sql, conn);
            command.ExecuteNonQuery();
            conn.Close();
            MessageBox.Show("Deletion completed!");
        }

        private void FindPacketsByMAC(SQLiteConnection conn, string mac)
        {
            conn.Open();
            String sql = "SELECT * FROM Data WHERE MAC ='" + mac + "';";
            SQLiteCommand command = new SQLiteCommand(sql, conn);
            SQLiteDataReader reader = command.ExecuteReader();
            int i = 0;
            String output = "";
            while (reader.Read())
            {
                 i++;
                output = output + i + ": MAC: " + reader["mac"] + "\tSSID: " + reader["SSID"] + "\ttime: " + reader["time"] + "\n";
            }
            if (i == 0)
            {
                output = "No items found";
            }
            MessageBox.Show(output);
            conn.Close();
        }

        private void FillDatabase(SQLiteConnection conn)
        {
            conn.Open();
            String sql = "INSERT INTO Data(MAC, SSID, time) VALUES('a','SSID','2018-04-28 08:23:19.120');";
            SQLiteCommand command = new SQLiteCommand(sql, conn);
            command.ExecuteNonQuery();
            MessageBox.Show("Inserted item!");
            conn.Close();
        }

        private void CreateTable(SQLiteConnection conn)
        {
            conn.Open();
            String sqlDrop = "DROP TABLE IF EXISTS Data;";
            String sqlCreate = "CREATE TABLE Data ("
                + " id integer PRIMARY KEY, "
                + " SSID text, "
                + " MAC text NOT NULL, "
                + " time TIMESTAMP NOT NULL"

                /*we don't have them right now
                + " hash "
                + " sgnlLevel"*/                
                + ");";
            SQLiteCommand command = new SQLiteCommand(sqlDrop, conn);
            command.ExecuteNonQuery();
            command = new SQLiteCommand(sqlCreate, conn);
            command.ExecuteNonQuery();
            MessageBox.Show("Table created!");
            conn.Close();
        }


    }
}