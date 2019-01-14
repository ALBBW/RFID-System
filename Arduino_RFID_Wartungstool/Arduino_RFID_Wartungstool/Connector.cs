using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;

namespace Arduino_RFID_Wartungstool
{
    class Connector
    {
        SerialPort sp = new SerialPort("COM3", 9600);
        
        public string Connect(string command)
        {
            if(!sp.IsOpen)
            {
                sp.Open();
            }
            if(command != "")
            {
                sp.WriteLine("s");
                sp.Write("s");
            }
            while(sp.IsOpen)
            {
                string input = sp.ReadLine();
                return input;
            }
            return "END";       
        }
    }
}
