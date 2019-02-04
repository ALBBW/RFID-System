/*
 * Programmname: Arduino_TCP.ino
 * Autor.......: Phillip-Morton Paape <phillip_morton@yahoo.com>
 * Datum.......: 28.01.2019
 * 
 * Beschreibung: Arduino der RFIDs Auslesen kann diese zu JSON umwandelt und per TCP an einen Server sendet.
 */

#include <Ethernet.h>
#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 8
#define SS_PIN 9

byte rfidCode[4];
String rfid = "";
bool isini = false;


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };      //Mac Adresse des Arduino
IPAddress ip(169, 254, 108, 237);                         //IP-Adresse des Arduino
IPAddress ipserver(169, 254, 108, 236);                   //IP-Adresse des Servers

EthernetClient client;                                    //Instanziiren des EthernetClient

bool issenden = false;

void setup() 
{
  Serial.begin(9600);                                     //Starten der Seriellen Ausgabe
  SPI.begin();
  Ethernet.begin(mac, ip);                                //Starten des Ethernets
  client.setConnectionTimeout(1);
  ncgt:
  delay(1000);
  
  if(client.connect(ipserver, 42000))                     //Verbinden mit dem Server
  {

  }
  else
  {
    Serial.println("Verbinden fehlgeschlagen");
    if(Ethernet.linkStatus() == Unknown)
    {
      Serial.println("Ethernet Kabel nicht angeschlossen");
    }
    //goto ncgt; //Da ist nichts bitte weiter gehen
  }
  Serial.println("Verbunden");
}

void loop() 
{
  if(!issenden)
  {
    if(rfid == "")
    {
      MFRC522 mfrc522(SS_PIN, RST_PIN);
      MFRC522::MIFARE_Key key;
      mfrc522.PCD_Init();
      while(rfid == "")
      {
        notread:
  
        if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
        {
          delay(50);
          Serial.println("Warte auf RFID");
          goto notread;
        }
  
        Serial.print(F("Card UID:"));
        byte counter = 0;
        for(byte i = 0; i < mfrc522.uid.size; i++)
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          rfid += (String)mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
          rfid += (String)mfrc522.uid.uidByte[i], HEX;
          rfidCode[i] = rfid.toInt();
          rfid = "";
          counter++;
          Serial.println();
        }
        for(int i = 0; i < counter; i++)
        {
          rfid += (String)rfidCode[i];
        }
      }
    }
  
  

    String ipstr = (String)ip[0] + "." + (String)ip[1] + "." + (String)ip[2] + "." + (String)ip[3];   //IP-Adresse des Arduino zu String fuer JSON
    
  
    /*
     * Bauen des JSON Strings
     */
    String jsonStr = "[\n";
           jsonStr += "\t{\n";
           jsonStr += "\t\t\"sender\": \"" + ipstr + "\",\n";
           jsonStr += "\t\t\"port\": \"1\",\n";
           jsonStr += "\t\t\"reason\": \"insert\"\n";
           jsonStr += "\t},\n";
           jsonStr += "\t{\n";
           jsonStr += "\t\t\"uid\": \"" + rfid + "\"\n";
           jsonStr += "\t}\n";
           jsonStr += "]";
    Serial.println(jsonStr);
    client.println(jsonStr);                          //Senden des JSON Strings
    rfid = "";
    issenden = true;
  }
  else
  {
    Serial.println("Warte auf antwort");
  }
         
  
  
  /*while(!client.available())
  {
    
  }
  int streamLength = client.available();            //Uebernehme laenge des erhaltenen Streams
  char readStream[streamLength];                    //Deklariere char Array mit laenge der Stream laenge
  for(int i = 0; i < streamLength; i++)
  {
    readStream[i] = client.read();                  //Lese erhaltenen Stream
  }
  String empfangen = "";                            //Deklariere und Initialisiere (Leere String Variable)
  for(int i = 0; i < streamLength; i++)
  {
    empfangen += readStream[i];                     //Fuege chars zu String zusammen
  }
  Serial.println(empfangen);                        //Gebe erhaltene antwort Seriell aus*/
  delay(5000);                                      //Warte 5 Sekunden
}
