using System;
using System.IO.Ports;
using System.Text.RegularExpressions;
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

            Port = new SerialPort(PortName, 9600, Parity.None, 8, StopBits.One);
            Port.Open();
        }

        public bool InitSystem ()
        {
            Thread.Sleep(2000);

            string command = $"INIT-{Frequency}{Environment.NewLine}";

            Port.Write(command);

            return Regex.Replace(Port.ReadLine(), @"\r\n?|\n", "").Equals("1");
        }

        public bool ToggleDirection (string direction)
        {
            string command = $"DIRECTION-{direction}{Environment.NewLine}";

            Port.Write(command);

            return Regex.Replace(Port.ReadLine(), @"\r\n?|\n", "").Equals("1");
        }

        public int MoveHorizontal (int steps, string direction)
        {
            if (ToggleDirection(direction))
            {
                string command = $"MOVE_HORIZONTAL-{steps}{Environment.NewLine}";

                Port.Write(command);

                return Int32.Parse(Regex.Replace(Port.ReadLine(), @"\r\n?|\n", ""));
            }
            return 0;
        }


        public int MoveVertical(int steps, string direction)
        {
            if (ToggleDirection(direction))
            {
                string command = $"MOVE_VERTICAL-{steps}{Environment.NewLine}";

                Port.Write(command);

                return Int32.Parse(Regex.Replace(Port.ReadLine(), @"\r\n?|\n", ""));
            }
            return 0;
        }

        //public static void CommandsHandler(Scanner instance)
        //{
        //    string command = Console.ReadLine();

        //    if (command.Equals("EXIT"))
        //    {
        //        Port.Close();
        //    }
        //    else
        //    {
        //        long unixTimestampStart = DateTimeOffset.Now.ToUnixTimeMilliseconds();

        //        Port.WriteLine(command);
        //        Console.WriteLine("Arduino response " + Port.ReadLine());

        //        long unixTimestampStop = DateTimeOffset.Now.ToUnixTimeMilliseconds();
        //        Console.WriteLine("Time " + (unixTimestampStop - unixTimestampStart));

        //        Scanner.CommandsHandler(instance);
        //    }
        //}


        public string ResultParser ()
        {
            return "";
        }
    }


    class MainClass
    {
        protected static string PortName = "/dev/tty.wchusbserial1420";


        public static void Main(string[] args)
        {
            Scanner scanner = new Scanner(PortName, 300.0);
            Console.WriteLine(scanner.InitSystem());
            Console.WriteLine(scanner.ToggleDirection("RIGHT"));
            Console.WriteLine(scanner.ToggleDirection("LEFT"));

            scanner.MoveHorizontal(100, "RIGHT");
        }

        static async Task<int> HandleFileAsync()
        {
            await Task.Delay(3000);
            return 100;
        }
    }
}
