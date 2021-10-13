#include <HTTPClient.h>

void Toroffner()
{
  ttgo->tft->fillScreen(TFT_BLACK);

  HTTPClient http;
  
  const char* Name       = "Toröffner";
  const char* Passwort   = "Toreinfahrt";
  
  WiFi.begin(Name, Passwort);

  while(!verbundenWlan())
  {
    if(physischerKnopf())
    {
      break;
    }
  }

  if(verbundenWlan())
  {
    ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);
    ttgo->tft->drawString("verbunden",  35, 30, 2);
  }
  
  int16_t x, y;
  while(verbundenWlan())
  {
    if(physischerKnopf())
    {
      break;
    }

    if(ttgo->getTouch(x, y))
    {
      
      http.begin("http://192.168.4.1:80/?pin=FUNCTION1ON");

      int httpCode = http.GET();

      http.end();
    }
  }
  trenneWlan();
  ttgo->tft->fillScreen(TFT_BLACK);
}
