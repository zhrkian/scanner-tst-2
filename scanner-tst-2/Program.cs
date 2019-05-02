using System;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace scannertst2
{
    class MainClass
    {
        protected static string PortName = "/dev/tty.wchusbserial1420";
        static SerialPort Port;


        public static void Main(string[] args)
        {
            Port = new SerialPort(PortName, 9600, Parity.None, 8, StopBits.One);

            Port.Open();

            CommandsHandler();
        }

        static void CommandsHandler()
        {
            string command = Console.ReadLine();

            if (command.Equals("EXIT"))
            {
                Port.Close();
            } else
            {
                Port.WriteLine(command);
                Console.WriteLine("Arduino response " + Port.ReadLine());
                CommandsHandler();
            }
        }

        static async Task<int> HandleFileAsync()
        {
            await Task.Delay(3000);
            return 100;
        }
    }
}
