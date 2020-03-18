/**
 * UDP Server and Byte decoder
 * 
 * @author Jann Bernlohr (Gamefreak) 
 * @version 1.0-12.02.2016 (Thats February you scrubs :3)
 */

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class server {
	 private byte[] inData = new byte[18];				//Maximum size of Data useable (shouldnt be more that 18 anyways)
	 private DatagramSocket socket;
	 private MousePlacer mouse;							//View class MousePlacer
	 private int X = 0;									//X Coordinate of Tablet (App)
	 private int Y = 0;									//Y Coordinate of Tablet (App)
	 private int pressure = 0;							//Pressure on Screen (App only available with pen support on screen)
	 
	public server(){
		consoleout("GfxTablet as jar by Jann Bernlohr (Gamefreak) \n Enjoy it!");	// :3 meow
		mouse = new MousePlacer(this);
		try {
			consoleout("Your IP-Address (use in App without verything in front of the /): "+Inet4Address.getLocalHost().toString());	//Give the IP-Address in console for user-friendlyness
		} catch (UnknownHostException e1) {e1.printStackTrace();}
		try {
			socket  = new DatagramSocket(40118);									//New Socket for Port 40118 (used by App)
		} catch (SocketException e) {e.printStackTrace();}
		while(true){
			DatagramPacket in = new DatagramPacket(inData,inData.length);			//Create new package for fruther use
			try {
				socket.receive(in);
			} catch (IOException e) {e.printStackTrace();}
			decode(in.getData());													//See method decode
			mouse.update(X,Y,pressure);												//Place mosue on Laptop/PC screen (see Class MousePlacer)
		}
		
	}
	public static void main(String[] args){
		new server();
	}
	
	/*
	 * Class for better console output (Time of output displayed)
	 */
	public void consoleout(String message){
		Date dNow = new Date( );
	    SimpleDateFormat ft = new SimpleDateFormat ("hh:mm:ss.S");
		System.out.println("["+ft.format(dNow)+"] "+message);
	}
	
	/*
	 * pretty self-explanatory I hope (Convert the 2 bytes of each short (int16) into an useable Integer (int32)
	 */
	public void decode(byte data[]){
		if(data.length>11){
			X = Short.toUnsignedInt((short)(((data[12] & 0xFF) << 8) | (data[13] & 0xFF)));
			Y = Short.toUnsignedInt((short)(((data[14] & 0xFF) << 8) | (data[15] & 0xFF)));
			pressure = Short.toUnsignedInt((short)(((data[16] & 0xFF) << 8) | (data[17] & 0xFF)));
			consoleout("X: "+X+"   Y: "+Y+"   pressure: "+pressure);										//Give Information of package
		}
	}
}
