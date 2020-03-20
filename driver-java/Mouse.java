// Controls Mouse functions of a PC

//Exception
import java.awt.AWTException;
//For getting screen
import java.awt.GraphicsDevice;
//For acquiring screen coords
import java.awt.GraphicsEnvironment;
//For being able to control devices
import java.awt.Robot;
//For mouse functionality
import java.awt.event.InputEvent;

public class Mouse {
	//Highest number that can be represented by unsigned 16bit numbers
	private final int sixt = 65535;
	//Maximum width/height of a PC screen in pixels
	private double MAX_X, MAX_Y;
	//Multiplicator in order for data to be correctly displayed on each screen resolution
	private double mulX, mulY;
	//Robot to control the mouse functions of the PC
	private Robot bot;
	//Server object used for console outputs
	private Server server;

	//Constructor
	public Mouse(Server server) {

		//Try creating a robot object
		try {
			bot = new Robot();
		} catch (AWTException e) {
			e.printStackTrace();
		}

		//Server will get the given server value
		this.server = server;
		//And we calibrate
		Calibrate();
	}

	//Calibrate all the necessities
	public void Calibrate() {
		//Create a GraphicsDevice to acquire screen coordinates
		GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
		//Acquire width and height
		MAX_X = gd.getDisplayMode().getWidth();
		MAX_Y = gd.getDisplayMode().getHeight();
		//Division by highest number that can be represented by 16-bit unsigned numbers
		mulX = MAX_X / getSixt();
		mulY = MAX_Y / getSixt();
		//We print results to console of server
		//format: WIDTHxHEIGHT
		this.server.print("Screen size: " + MAX_X + "x" + MAX_Y);
		//format: X: NUMBER || Y : NUMBER
		this.server.print("Calibration of Multiplicators|| X: " + mulX + "||   Y: " + mulY);
	}

	//For making the robot object control mouse functionality accordingly:
	public void Update(int pX, int pY, int pressure, boolean left, boolean right) {
		//Use multiplicators on the coordinates and control mouse
		this.bot.mouseMove((int)(pX * mulX),(int)(pY * mulY));
		//MousePress/MouseRelease coming soon
		//dumb pressure solution
		if (left == true) {
			bot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
		}
		if (left == false) {
			bot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
		}
	}

	// Constant getter
	public int getSixt() {
		return this.sixt;
	}
}