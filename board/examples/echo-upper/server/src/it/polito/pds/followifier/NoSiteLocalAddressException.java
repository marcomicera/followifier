package it.polito.pds.followifier;

public class NoSiteLocalAddressException extends Exception {

	private static final long serialVersionUID = 4578955349209502840L;

	public String toString() {
		return "No site-local address found on this machine.";
	}
}
