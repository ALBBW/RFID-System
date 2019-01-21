#include <MFRC522.h>
#include <Ethernet.h>
#include "Formater.h"
//--------------------#include <ArduinoJson.h>

//String message;
int keycode[200];
char ascii[200];
int counter = 0;
bool writer = false;
bool disabled = false;
//bool isende = false;
String text = "";
String test = "";
String rfidcode = "";

//Ethernet Variablen
byte ip[] = { 192, 168, 1, 42};                   //IP-Adresse des Arduino
String ipstr = "192.168.1.42";
byte subnet[] = { 255, 255, 255, 0};              //Subnetzmaske des Arduino
byte mac[] = { 0xAD, 0xBE, 0xFE, 0xED};           //Mac-Adresse des Arduino

//RFID Variablen
#define RST_PIN 9                                 //Reset Pin fuers RFID Board
#define SS_PIN 10                                 //Kommunikation Pin fuers RFID Board
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}
MFRC522 rfidModul(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

EthernetClient client = EthernetClient(42000);    
IPAddress ipaddr(10, 122, 122, 193);               //IP-Adresse des Servers
//10.122.122.193

void setup() 
{
  Serial.begin(9600);                                                                                                             //Starte Serielle Ausgabe
  Serial.println("Arduino RFID-Module @ " + (String)ip[0] + "." + (String)ip[1] + "." + (String)ip[2] + "." + (String)ip[3]);     //Ausgabe des Modulnamens + IP-Adresse
  Serial.println("Initilaize RFID-Module");
  SPI.begin();                                                                                                                    //Starte SPI
  rfidModul.PCD_Init();                                                                                                           //Initialisiere RFID-Modul
  for(byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println("RFID-Module ready");
  if(Ethernet.begin(mac) == 0)                              //Starte Ethernet und pruefe Zeitgleich ob Verbindung erfolgreich
  {
    Serial.println("DHCP Fail");                            //Falls Verbindung nicht erfolgreich ist eine IP zuordnung mitteles DHCP Fehlgeschlagen
    Ethernet.begin(mac, ip);                                //dann versuche Ethernet erneut zu starten diesmal wird IP mit uebergeben
  }
  Serial.println("Initialize Shield");                      //Ausgabe das er Initialisiert
  delay(1000);                                              //1 Sekunde Initialisierungszeit wird bereitgestellt
  Serial.println("connecting . . .");                       //Ausgabe das er versucht eine verbindung aufzubauen
  if(client.connected())                                    //Pruefen ob verbindung hergestellt wurde
  {
    Serial.println("Connected");                            //Ausgabe wenn Verbindung erfolgreich
    /*
     * Sende Request an Server
     */
    client.write((byte)0x62);
    client.write((byte)0xff);
    client.write((byte)0x03);
    client.write((byte)0xff);
    client.write((byte)0x00);
    client.write((byte)0x45);
    client.write((byte)0x58);
    client.write((byte)0x00);
  }
  else
  {
    Serial.println("Connection failed!");                     //Ausgabe wenn Verbindung nicht erfolgreich ist
  }
}

void loop() 
{  
  delay(50);
  
  while(Serial.available() > 0)      
  {                                  
    keycode[counter] = Serial.read();
    test = Serial.read();
    KeytoChar();
    counter++;
  }        

  if(text == "s")
  {
    if(!disabled)
    {
      disabled = true;
      Serial.println("Gerät wurde Deaktiviert");
      text = "";
    }
    else
    {
      disabled = false;
      Serial.println("Gerät wurde Aktiviert");
      text = "";
    }
  }
  else
  {
    text = "";
  }
  
  if(disabled)
  {
    return;
  }
  
  Serial.println("warte bis Karte gelesen");
  
  /*
   * Pruefe ob Karte gelesen wurde und ob es eine neue Karte ist
   */
  if(!rfidModul.PICC_IsNewCardPresent())
  {
    return;
  }

  if(!rfidModul.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print(F("Card UID: "));
  dump_byte_array(rfidModul.uid.uidByte, rfidModul.uid.size);       //Methode zum Auslesen des RFID-Chips
  Serial.println(rfidcode);                                         //Gebe RFID-Nummer Seriell Aus

  if(writer == true)                                    
  {
    String JSONText = MakeJson(rfidcode, ipstr);               //Mache RFID-Nummer zu JSON
    if(JSONText != "")                                  //ist JSON nicht leer
    {
      int msgLength = CheckJsonLength(JSONText);        //Ermittel die Laenge des JSON Strings
      byte message[msgLength];                          //Deklariere Unsigned 8 Bit Byte Array mit der Laenge des JSON Strings
      for(int i = 0; i < msgLength; i++)
      {
        message[i] = (byte)JSONText[i];                 //Wandle JSON String zu Byte um
      }
      for(int i = 0; i < msgLength; i++)
      {
        client.write(message[i]);                       //Sende die einzelnen Bytes
      }
     }
     Serial.println("");  
    }
    writer = false;
    counter = 0;
    text = "";
}

void KeytoChar()
{ 
  //Serial.print(keycode[counter]);                                     //Code für die Ausgabe der Ascii-Zeichen!
  if(keycode[counter] == 10)
  {
    writer = true;
  }
  else 
  {
    int zaehler = 0;
    int zahl[100] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 252, 228, 246, 223};
    char buchstaben[100] = {' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'};
      
    while(zaehler < 99)
    {
      if(zahl[zaehler] == keycode[counter])
      {
        switch(zahl[zaehler])
        {
          case 252:
            text += ("ue");
          break;
  
          case 228:
            text += ("ae");
          break;
  
          case 246:
            text += ("oe");
          break;
  
          case 223:
            text += ("ss");
          break;

          default:
            text += buchstaben[zaehler];
            break;
        }         
      }
      zaehler++;
    }
  }
}

void dump_byte_array(byte *buffer, byte bufferSize)
{
  for(byte i = 0; i < bufferSize; i++)
  {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], HEX);
    rfidcode += buffer[i] < 0x10 ? " 0" : " ";
    rfidcode += buffer[i], HEX;
  }
}
