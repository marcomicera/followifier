package it.polito.pds.followifier.examples.db;

public class DataObject {
	public int id;
	public String MAC;
	public String SSID;
	
	public DataObject(int id, String MAC, String SSID) {
		this.id = id;
		this.MAC = MAC;
		this.SSID = SSID;
	}

	public void print() {
		System.out.println("SSID: "+SSID+", ID: "+id+", MAC: "+MAC);
	}
	
}
