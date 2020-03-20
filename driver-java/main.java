//Main class, puts everything into motion

//For allowing quitting(?)
import java.awt.event.InputEvent;

public class main {
	public static void main(String args[]) {
		//Create a server object
		Server server = new Server();
		//Start the loop so we can read for events
		server.Loop();
	}
}