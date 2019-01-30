/*
 * Programmname: Arduino_TCP.ino
 * Autor.......: Phillip-Morton Paape <phillip_morton@yahoo.com>
 * Datum.......: 28.01.2019
 * 
 * Beschreibung: Arduino der RFIDs Auslesen kann diese zu JSON umwandelt und per TCP an einen Server sendet.
 */

#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };      //Mac Adresse des Arduino
IPAddress ip(169, 254, 108, 237);                         //IP-Adresse des Arduino
IPAddress ipserver(169, 254, 108, 236);                   //IP-Adresse des Servers

EthernetClient client;                                    //Instanziiren des EthernetClient

bool issenden = false;

void setup() 
{
  Serial.begin(9600);                                     //Starten der Seriellen Ausgabe
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
    goto ncgt; //Da ist nichts bitte weiter gehen
  }
  Serial.println("Verbunden");
}

void loop() 
{
  delay(50);
  /*
   * Hier RFID Auslesen Implementieren
   */
  
  if(!issenden)
  {
    String rfid = "420-111-215";
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

    client.println(jsonStr);                          //Senden des JSON Strings
    issenden = true;
  }
  else
  {
    Serial.println("Warte auf antwort");
  }
         
  
  
  if(!client.available())
  {
    return;
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
  Serial.println(empfangen);                        //Gebe erhaltene antwort Seriell aus
  delay(5000);                                      //Warte 5 Sekunden
}
