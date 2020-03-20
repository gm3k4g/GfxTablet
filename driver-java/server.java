// UDP Server and byte decoder

import java.io.IOException;
import java.net.UnknownHostException;
import java.net.SocketException;
//UDP
import java.net.DatagramPacket;
import java.net.DatagramSocket;
//Getting your IPv4
import java.net.Inet4Address;
//For getting date
import java.text.SimpleDateFormat;
import java.util.Date;
//Mouse functionality
import java.awt.event.InputEvent;
import java.awt.Robot;

public class Server {
	// Maximum size of data received is 18 (according to the app)
	private byte[] inData = new byte[18];
	// UDP: We use Datagram, which is a self contained message (may or may not arrive).
	private DatagramSocket socket;
	// We'll be controlling a mouse
	private Mouse mouse;
	// X and Y Coordinates on tablet(the app)
	private int X;
	private int Y;
	// Pressure on tablet(the app). Only available
	// with pen support.
	private int pressure;
	// Left and right mouse presses.
	private boolean left;
	private boolean right;

	//Constructor
	//We initialize all the importants here.
	public Server() {
		//Some priorities
		setX(0);
		setY(0);
		setPressure(0);
		setLeft(false);
		setRight(false);

		//Startup message
		print("GfxTablet Java(Jar file) edition: Re-written by \"\" \n");
		//Give the mouse the server as function argument
		mouse = new Mouse(this);

		//Try getting IP. If this fails, something is probably not right in your computer
		try {
			//This will show your IP and you can 
			//immediately copy it on your phone/tablet.
			print("Copy the IP Address after the / (slash) symbol into your app on android: "
			+	Inet4Address.getLocalHost().toString());
		} catch (UnknownHostException e1) {
			e1.printStackTrace();
		}

		//Try using port 40118 for our UDP socket
		try {
			socket = new DatagramSocket(40118);
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}

	// This is the main loop, it checks for mouse events.
	public void Loop() {
		//Petition to re-do this with polling?
		//Start
		while(true) {
			// Here we are constantly acquiring the packets
			DatagramPacket in = new DatagramPacket(inData, inData.length);

			// Keep receiving events
			try {
				socket.receive(in);
			} catch (IOException e) {
				e.printStackTrace();
			}

			//Process what data we got from the socket
			Process(in.getData());
			//Update mouse accordingly
			this.mouse.Update(getX(),getY(),getPressure(),getLeft(),getRight());
		}
	}

	// Print stuff to console
	// Using this function to output date alongside the data.
	public void print(String msg) {
		Date present = new Date();
		SimpleDateFormat ft = new SimpleDateFormat("hh:mm:ss.S");
		System.out.println("[" + ft.format(present) + "]" + msg);
	}

	//Process data:
	// Mouse coords:
	// 		convert 2 bytes of each short(int16)
	// 		to a useable Integer (int32)
	// Pressure:
	//		 Same as above
	// Clicks:
	//		coming soon
	public void Process(byte data[]) {
		// Data length needs to be larger than 11
		if(data.length > 11) {
			//X and Y coordinate decoding
			setX(Short.toUnsignedInt((short)(((data[12] & 0xFF) << 8) | (data[13] & 0xFF))));
			setY(Short.toUnsignedInt((short)(((data[14] & 0xFF) << 8) | (data[15] & 0xFF))));
			//pressure decoding
			setPressure(Short.toUnsignedInt((short)(((data[16] & 0xFF) << 8) | (data[17] & 0xFF))));  
			//clicks coming soon
			//Stupid solution: use pressure to determine if we're clicking or not
			if( pressure > 3000 ) {
				setLeft(true);
			} else {
				setLeft(false);
			}

			//Then print everything out
			print("X: " + X + "|| Y : " + Y + "|| Left click: " + left + "|| Right click: " + right + "|| Pressure: " + pressure);
		}
	}

	/////////////
	//Getters/Setters

	//get X coord
	public int getX() {
		return this.X;
	}

	//set X coord
	public void setX(int X) {
		this.X = X;
	}

	//get Y coord
	public int getY() {
		return this.Y;
	}

	//set Y coord
	public void setY(int Y) {
		this.Y = Y;
	}

	//get pressure
	public int getPressure() {
		return this.pressure;
	}

	//set pressure
	public void setPressure(int pressure) {
		this.pressure = pressure;
	} 

	//get left mouse
	public boolean getLeft() {
		return this.left;
	}

	//set left mouse
	public void setLeft(boolean left) {
		this.left = left;
	}

	//get right mouse
	public boolean getRight() {
		return this.right;
	}

	//set right mouse
	public void setRight(boolean right) {
		this.right = right;
	}
}