using System.Net.Sockets;
using System.Text;

namespace Client
{
    class Client
    {
        const int PORT_NO = 8080;
        const string SERVER_IP = "192.168.38.45";
        static void Main(string[] args)
        {
            //---data to send to the server---
            string textToSend = "Using curve384, publicKey\n";
            string pubKeyx = "pubKeyHPx = 15823354386728477868059489378147753930914124451780717471229430887408529514490259865080695157434160844402037612752985\n";
            string pubKeyy = "pubKeyHPy = 14249003402157307118910319908225993651282273477814127020647122530930973296805790540669926590590574050849654965310718\n";

            //---create a TCPClient object at the IP and port no.---
            TcpClient client = new TcpClient(SERVER_IP, PORT_NO);
            NetworkStream nwStream = client.GetStream();
            byte[] bytesToSend = Encoding.ASCII.GetBytes(textToSend);
            byte[] pubKeyxToSend = Encoding.ASCII.GetBytes(pubKeyx);
            byte[] pubKeyyToSend = Encoding.ASCII.GetBytes(pubKeyy);

            //---send the text---
            Console.WriteLine("Sending : " + textToSend);
            nwStream.Write(bytesToSend, 0, bytesToSend.Length);
            Console.WriteLine("Sending : " + pubKeyx);
            nwStream.Write(pubKeyyToSend, 0, pubKeyxToSend.Length);
            Console.WriteLine("Sending : " + pubKeyy);
            nwStream.Write(pubKeyyToSend, 0, pubKeyyToSend.Length);

            //---read back the text---
            while (true)
            {
                byte[] bytesToRead = new byte[client.ReceiveBufferSize];
                int bytesRead = nwStream.Read(bytesToRead, 0, client.ReceiveBufferSize);
                string receivedStr = Encoding.ASCII.GetString(bytesToRead, 0, bytesRead);
                Console.WriteLine("Received : " + receivedStr);
                //Console.ReadLine();

                if(receivedStr.Contains("New client connected"))
                {
                    Console.WriteLine("Sending : " + textToSend);
                    nwStream.Write(bytesToSend, 0, bytesToSend.Length);
                    Console.WriteLine("Sending : " + pubKeyx);
                    nwStream.Write(pubKeyyToSend, 0, pubKeyxToSend.Length);
                    Console.WriteLine("Sending : " + pubKeyy);
                    nwStream.Write(pubKeyyToSend, 0, pubKeyyToSend.Length);
                }
            }
            client.Close();
        }
    }
}