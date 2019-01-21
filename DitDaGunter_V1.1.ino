#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <Ethernet.h>
#include "Formater.h"
//--------------------#include <ArduinoJson.h>

String message;
int keycode[200];
char ascii[200];
int counter = 0;
bool writer = false;
//bool isende = false;
String text = "";
int zaehler = 1;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Arduino RFID-Module");
}

void loop() 
{  
  while(Serial.available() > 0)
  {
    keycode[counter] = Serial.read();
    KeytoChar();
    counter++;
  }

  if(writer == true)
  {
    if(zaehler < 10)
    {
      Serial.print("0"); 
    }
    Serial.print(zaehler); 
    Serial.print(". ");
    Serial.print(text);
    Serial.println("");
    String JsonTxt = MakeJson(text);
    Serial.println(JsonTxt);
    writer = false;
    counter = 0;
    text = ""; 
    zaehler++;
  }
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
