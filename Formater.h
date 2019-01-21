String MakeJson(String content, String ipaddrs)
{
  if(content == "")
  {
    return "";
  }
  int leers = 0;
  for(int i = 0; i < content.length(); i++)
  {
    switch(content[i])
    {
      case ' ':
        leers++;
        break;
      default:
        break;
    }
  }
  leers++;
  String words[leers];
  String currword = "";
  int selwrd = 0;
  for(int i = 0; i < content.length(); i++)
  {
    switch(content[i])
    {
      case ' ':
        words[selwrd] = currword;
        selwrd++;
        currword = "";
        break; 
      default:
        currword += content[i];
        break;
    }
  }
  words[selwrd] = currword;
  Serial.println(currword);
  String JsonStr = "{\n\"Identifier\": \"Arduino\"\n\"IP-Adress\": \"" + ipaddrs + "\"\"Content\": [\n";
  for(int i = 0; i < leers; i++)
  {
    JsonStr += "\"" + words[i] + "\",\n";
  }
  JsonStr += "],\n}";
  return JsonStr;
}

int CheckJsonLength(String json)
{
  bool isend = false;
  int counter = 0;
  while(isend == false)
  {
    switch(json[counter])
    {
      case '}':
        isend = true;
        break;
       default:
        break;
    }
    counter++;
  }
  return counter;
}
