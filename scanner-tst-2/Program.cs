using System;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;


namespace scannertst2
{
    class Scanner
    {
        public string PortName;
        public double Frequency;
        static SerialPort Port;

        public Scanner (string portName, double frequency)
        {
            PortName = portName;
            Frequency = frequency;
        }

        public bool InitSystem ()
        {
            string result = "";
            string command = $"INIT-{Frequency}\n";
            Console.WriteLine(command);
            Port = new SerialPort(PortName, 9600, Parity.None, 8, StopBits.One);
            Port.Open();
            //Port.NewLine = "\r\n";  //Environment.NewLine;
            Port.Write(command);
            result = Port.ReadLine();

            Console.WriteLine(result);

            if (result.Equals("true"))
            {
                return true;
            }

            return false;
        }

        public string ResultParser ()
        {
            return "";
        }
    }


    class MainClass
    {
        protected static string PortName = "/dev/tty.wchusbserial1420";
        //static SerialPort Port;


        public static void Main(string[] args)
        {
            //String[] ports = SerialPort.GetPortNames();

            //for (int i = 0; i < ports.Length; i++)
            //{
            //    Console.WriteLine(ports[i]);
            //}
            Scanner scanner = new Scanner(PortName, 300.0);

            Console.WriteLine("TRY TO INIT");

            Console.WriteLine(scanner.InitSystem());

            //Port = new SerialPort(PortName, 9600, Parity.None, 8, StopBits.One);

            //Port.Open();

            //CommandsHandler();


        }

        static void CommandsHandler()
        {
            //string command = Console.ReadLine();

            //if (command.Equals("EXIT"))
            //{
            //    Port.Close();
            //} else
            //{
            //    long unixTimestampStart = DateTimeOffset.Now.ToUnixTimeMilliseconds();

            //    Port.WriteLine(command);
            //    Console.WriteLine("Arduino response " + Port.ReadLine());

            //    long unixTimestampStop = DateTimeOffset.Now.ToUnixTimeMilliseconds();
            //    Console.WriteLine("Time " + (unixTimestampStop - unixTimestampStart));

            //    CommandsHandler();
            //}
        }

        static async Task<int> HandleFileAsync()
        {
            await Task.Delay(3000);
            return 100;
        }
    }
}
