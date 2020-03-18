/**
 * Places the mouse on the given coordinates
 * 
 * @author Jann Bernlohr (Gamefreak) 
 * @version 1.0-12.02.2016 (Thats February you scrubs :3)
 */
import java.awt.AWTException;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Robot;

public class MousePlacer {
	private double MAX_X, MAX_Y;			//Maximum width/height of PC/Laptop screen (in pixels of course)
	private double mult_X, mult_Y;			//Multiplicator for data to be displayed correctly on each screen resolution
	private Robot robot;					//Robot to place the mouse
	private server server;					//Only used for console outputs
	
	public MousePlacer(server pServer){
		try {robot = new Robot();} catch (AWTException e) {e.printStackTrace();}
		server = pServer;
		calibrate();								//See method calibrate
	}
	
	/*
	 * Pretty much completly explained above in the global variables
	 */
	public void calibrate(){
		GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
		MAX_X = gd.getDisplayMode().getWidth();
		MAX_Y = gd.getDisplayMode().getHeight();
		mult_X = MAX_X / 65535;
		mult_Y = MAX_Y / 65535;
		server.consoleout("Screen size: "+MAX_X+"x"+MAX_Y);
		server.consoleout("Multiplicator to calibrate X: " + mult_X+"   Y: "+mult_Y);
		
	}
	/*
	 * Pressure not used (yet)
	 */
	public void update(int pX, int pY,int pPressure){
		double X = pX* mult_X;				//Get the right coordinates for the mouse placement
		double Y = pY * mult_Y;				//Get the right coordinates for the mouse placement
		robot.mouseMove((int)X,(int)Y);		//Place the mouse on the screen
	}
}
