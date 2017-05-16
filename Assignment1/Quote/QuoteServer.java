import java.net.*;
import java.io.*;
import java.util.Random;
public class QuoteServer
{
	public static void main(String[] args)
	{
		try{
			Random rn = new Random();
			ServerSocket sock = new ServerSocket(6017);

			while(true){
				Socket client = sock.accept();
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				pout.println(Quote.quoteGenerator[rn.nextInt(5)]);
				client.close();
			}

		}
		catch(IOException ioe){System.err.println(ioe);}
	}

}