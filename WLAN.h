#include <WiFi.h>

const char* ssid       = "FRITZ!Box 7490";
const char* password   = "09760476159909696007";

void verbindeWlan()
{
  WiFi.begin(ssid, password);
}

bool verbundenWlan()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void trenneWlan()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void starteAP()
{



  WiFi.softAP("TTGO", "ttgowatch");

  IPAddress IP = WiFi.softAPIP();


}
