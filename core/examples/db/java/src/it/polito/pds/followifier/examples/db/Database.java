package it.polito.pds.followifier.examples.db;

import java.sql.*;
import java.util.*;

public class Database {

	//connection handler and database creation
	private static Connection Connect() {
		String url = "jdbc:sqlite:database.db";
		Connection conn = null;
		try {
			conn = DriverManager.getConnection(url);
		}catch(SQLException e) {
			System.out.println(e.getMessage());
		}
		return conn;
	}

	//creation table for data
	public static void CreateTable() {
		String sqlDrop = "DROP TABLE IF EXISTS Data;";
	    
		String sqlCreate = "CREATE TABLE Data ("
				+ " id integer PRIMARY KEY, "
				+ " SSID text, "
				+ " MAC text NOT NULL, "
				+ " time TIMESTAMP NOT NULL"
				
				//we don't have them right now
/*				+ " hash "
				+ " sgnlLevel"
*/				+ ");";
		Connection conn = Connect();
		if(conn!=null) {
			try{
				Statement stmt = conn.createStatement();
				stmt.execute(sqlDrop);
				stmt.execute(sqlCreate);
				stmt.close();
				System.out.println("Table created");
				conn.close();
			}catch(SQLException e) {
				System.out.println(e.getMessage());
			}	
		}		
	}
	
	//fill database with data from Wireshark -- having issue getting probe requests from wireshark
	public static void FillDatabase() {
		String sql = "INSERT INTO Data(MAC, SSID, time) VALUES(?,?,?);";
		try {
			Connection conn = Connect();
			PreparedStatement stmt = conn.prepareStatement(sql);
			stmt.setString(1, "a");
			stmt.setString(2, "SSID");
			stmt.setTimestamp(3, new Timestamp(1));
			stmt.executeUpdate();
			conn.close();
		}catch(SQLException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public static void FindPacketsByMAC(String givenMAC) {
		String sql = "SELECT * FROM Data WHERE MAC = ?;";
		List<DataObject> list = new ArrayList<DataObject>();
		try {
			Connection conn = Connect();
			if(conn!=null) {
				PreparedStatement stmt = conn.prepareStatement(sql);
				stmt.setString(1, givenMAC);
				ResultSet rs = stmt.executeQuery();
				int i=0;
				while(rs.next()) {
					list.add(new DataObject(rs.getInt("id"), rs.getString("MAC"), rs.getString("SSID")));
					list.get(i).print();
					i++;
				}
				if(i==0) {
					System.out.println("No elements found");
				}
				stmt.close();
				conn.close();
			}
		}catch(SQLException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public static void DeleteByTimestamp(Timestamp time) {
		String sql = "DELETE FROM Data WHERE time < ?;";
		try {
			Connection conn = Connect();
			if(conn!=null) {
				PreparedStatement stmt = conn.prepareStatement(sql);
				stmt.setTimestamp(1, time);
				stmt.executeUpdate();
				conn.close();
			}
		}catch(SQLException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public static void main(String args[]) {
		CreateTable();
		FillDatabase();
		FillDatabase();
		FindPacketsByMAC("a");		
		DeleteByTimestamp(new Timestamp(2));
		FindPacketsByMAC("a");
	}
}
