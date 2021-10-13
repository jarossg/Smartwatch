uint8_t hh, mm, ss, mmonth, dday; // H, M, S variables
uint16_t yyear; // Year is 16 bit int



void displayTime(boolean fullUpdate)
{
    byte xpos = 40; // Stating position for the display
    byte ypos = 90;

    // Get the current data
    RTC_Date tnow = ttgo->rtc->getDateTime();

    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

    ttgo->tft->setTextSize(1);

    if (fullUpdate) {
        // Font 7 is a 7-seg display but only contains
        // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .

        ttgo->tft->setTextColor(0x39C4, TFT_BLACK); // Set desired color
        ttgo->tft->drawString("88:88", xpos, ypos, 7);
        ttgo->tft->setTextColor(0xFBE0, TFT_BLACK); // Orange

        if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        xpos += ttgo->tft->drawNumber(hh, xpos, ypos, 7);
        xcolon = xpos + 3;
        xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
        if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        ttgo->tft->drawNumber(mm, xpos, ypos, 7);
    }

    if (ss % 2) { // Toggle the colon every second
        ttgo->tft->setTextColor(0x39C4, TFT_BLACK);
        xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
        ttgo->tft->setTextColor(0xFBE0, TFT_BLACK);
    } else {
        ttgo->tft->drawChar(':', xcolon, ypos, 7);
    }
    ttgo->tft->setTextSize(3);
    ttgo->tft->setCursor( 10, 210);

    
    //Datumsanzeige
    ttgo->tft->print(dday);
    ttgo->tft->print(".");
    ttgo->tft->print(mmonth);
    ttgo->tft->print(".");
    ttgo->tft->print(yyear);

    
    //Akkustand
    if(ttgo->power->getBattPercentage()>99)
    {
      ttgo->tft->setCursor( 160, 5);
    }
    else
    {
      ttgo->tft->setCursor( 180, 5);
    }
    ttgo->tft->print(ttgo->power->getBattPercentage());
    ttgo->tft->print("%");

    //Schlafenlegen
    if(physischerKnopf())
    {
        ttgo->powerOff();
        esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);

        esp_deep_sleep_start();
    }
}

// prtTime will display the current selected time and highlight
// the current digit to be updated in yellow

void prtTime(byte digit)
{
    ttgo->tft->fillRect(0, 0, 100, 34, TFT_BLACK);
    if (digit == 1)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(int(hh / 10), 5, 5, 2);
    if (digit == 2)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(hh % 10, 25, 5, 2);
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawString(":",  45, 5, 2);
    if (digit == 3)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(int(mm / 10), 65, 5, 2);
    if (digit == 4)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(mm % 10, 85, 5, 2);
}

// getTnum takes care of translating where we pressed into
// a number that was pressed. Returns -1 for no press
// and 13 for DONE

int getTnum()
{
    int16_t x, y;
    if (!ttgo->getTouch(x, y)) return - 1;
    if (y < 85) {
        if (x < 80) return 1;
        else if (x > 160) return 3;
        else return 2;
    } else if (y < 135) {
        if (x < 80) return 4;
        else if (x > 160) return 6;
        else return 5;
    } else if (y < 185) {
        if (x < 80) return 7;
        else if (x > 160) return 9;
        else return 8;
    } else if (x < 80) return 0;
    else return 13;
}

int gibZeitTaste()
{
    int16_t x, y;
    if (!ttgo->getTouch(x, y)) return - 1;

    int Taste;
    if (y < 120) 
    {
        if (x < 60) Taste = 0;
        else if (x > 60 && x < 120) Taste = 1;
        else if (x > 120 && x < 180) Taste = 2;
        else Taste = 3;
    } 
    else if (y < 220) 
    {
        if (x < 60) Taste = 4;
        else if (x > 60 && x < 120) Taste = 5;
        else if (x > 120 && x < 180) Taste = 6;
        else Taste = 7;
    }    
    else Taste = 13;
    while (ttgo->getTouch(x, y)) {}
    return Taste;
}

void Uhrstellen()
{
       ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->drawLine(60, 0, 60, 240, 52760);
    ttgo->tft->drawLine(120, 0, 120, 240, 52760);
    ttgo->tft->drawLine(180, 0, 180, 240, 52760);

    ttgo->tft->drawLine(0, 120, 240, 120, 52760);
    //ttgo->tft->drawLine(0, 200, 240, 200, 52760);
    /*
    ttgo->tft->fillRoundRect(120, 200, 119, 39, 6, TFT_WHITE);
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor(0, 0);

    ttgo->tft->setCursor(155, 210);
    ttgo->tft->setTextColor(TFT_BLACK);
    ttgo->tft->print("DONE");
    */
    RTC_Date tnow = ttgo->rtc->getDateTime();

    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

    ttgo->tft->setTextSize(1);
        // Font 7 is a 7-seg display but only contains
        // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .

        ttgo->tft->setTextColor(0x39C4, TFT_BLACK); // Set desired color
        ttgo->tft->drawString("88:88", 40, 90, 7);
        ttgo->tft->setTextColor(0xFBE0, TFT_BLACK); // Orange
    while(!physischerKnopf())
    {
         byte xpos = 40; // Stating position for the display
        byte ypos = 90;
    
        if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        xpos += ttgo->tft->drawNumber(hh, xpos, ypos, 7);
        xcolon = xpos + 3;
        xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
        if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        ttgo->tft->drawNumber(mm, xpos, ypos, 7);
        
        int Taste = gibZeitTaste();
        switch (Taste) { 
        case 0: 
            if(hh < 24)hh = hh + 10;
            else hh = 0;
            break;
        case 1:
            if(hh < 24)hh = hh + 1;
            else hh = 0;
            break;
        case 2:
            if(mm < 60)mm = mm + 10;
            else mm = 0;
            break;
        case 3:
            if(mm < 60)mm = mm + 1;
            else mm = 0;
            break;
        case 4:
            if(hh > 0)hh = hh - 10;
            else hh = 24;
            break;
        case 5:
            if(hh > 0)hh = hh - 1;
            else hh = 24;
            break;
        case 6:
            if(mm > 0)mm = mm - 10;
            else mm = 60;
            break;
        case 7:
            if(mm > 0)mm = mm - 1;
            else mm = 60;
            
            break;            
        }

        
    }

    ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
}





// An advanced Time Display
// This is a drop-in replacement for the displayTime()
// In the original Instructable. s such, it redraws GUI
// every minute so you will see a little flicker.

/*
void displayTime(boolean fullUpdate) {
  ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);

  // Get the current data
  RTC_Date tnow = ttgo->rtc->getDateTime();

  hh = tnow.hour;
  mm = tnow.minute;
  ss = tnow.second;
  dday = tnow.day;
  mmonth = tnow.month;
  yyear = tnow.year;

  ttgo->tft->setTextSize(1);

  if (fullUpdate) {

    //Draw the back graphics - Top of display
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->fillRoundRect(0, 0, 239, 120, 40, TFT_PURPLE);
    ttgo->tft->fillRoundRect(40, 20, 196, 80, 20, TFT_BLACK);
    ttgo->tft->fillRect(80, 20, 159, 80, TFT_BLACK);
    ttgo->tft->fillRect(170, 0, 45, 20, TFT_BLACK);
    ttgo->tft->fillRect(110, 0, 4, 20, TFT_BLACK);
    ttgo->tft->fillRect(0, 45, 50, 7, TFT_BLACK);
    ttgo->tft->fillRect(0, 70, 50, 7, TFT_BLACK);
    ttgo->tft->fillRect(215, 0, 24, 20, TFT_DARKCYAN);

    //Draw the back graphics - Bottom of display
    ttgo->tft->fillRoundRect(0, 130, 239, 109, 40, TFT_MAROON);
    ttgo->tft->fillRoundRect(40, 150, 199, 88, 20, TFT_BLACK);
    ttgo->tft->fillRect(0, 179, 50, 10, TFT_BLACK);
    ttgo->tft->fillRect(100, 160, 40, 10, TFT_YELLOW);
    ttgo->tft->fillRect(140, 160, 40, 10, TFT_DARKGREEN);
    ttgo->tft->fillRect(180, 160, 40, 10, TFT_RED);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->drawString("Temp", 66, 158, 2);
    ttgo->tft->fillRoundRect(119, 210, 120, 29, 15, TFT_DARKCYAN);

    // Display Temp Marker - you may need to adjust the x value based on your normal ADC results
    float temp = ttgo->power->getTemp();
    ttgo->tft->fillRoundRect(int(temp), 170, 10, 20, 5, TFT_WHITE);

    // Display Time
    // Font 7 is a 7-seg display but only contains
    // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .

    ttgo->tft->setTextColor(0xFBE0, TFT_BLACK);
    int xpos = 55;
    if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, 35, 7);
    xpos += ttgo->tft->drawNumber(hh, xpos, 35, 7);
    xpos += 3;
    xpos += ttgo->tft->drawChar(':', xpos, 35, 7);
    if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, 35, 7);
    ttgo->tft->drawNumber(mm, xpos, 35, 7);

    // Display Battery Level
    ttgo->tft->setTextSize(1);
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->drawString("Power", 124, 2, 2);
    ttgo->tft->setTextColor(TFT_GREEN);
    int per = ttgo->power->getBattPercentage();
    //per = ttgo->power->getBattPercentage();
    ttgo->tft->drawString(String(per) + "%", 179, 2, 2);
    ttgo->tft->setTextColor(TFT_GREENYELLOW);
    ttgo->tft->drawString(String(dday), 50, 188, 6);

    // Turn off the battery adc
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, false);

    // Draw Month
    String mStr;
    switch (mmonth) {
      case 1: mStr = "Jan"; break;
      case 2: mStr = "Feb"; break;
      case 3: mStr = "Mar"; break;
      case 4: mStr = "Apr"; break;
      case 5: mStr = "May"; break;
      case 6: mStr = "Jun"; break;
      case 7: mStr = "Jul"; break;
      case 8: mStr = "Aug"; break;
      case 9: mStr = "Sep"; break;
      case 10: mStr = "Oct"; break;
      case 11: mStr = "Nov"; break;
      case 12: mStr = "Dec"; break;
    }
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawString(mStr, 9, 194, 2);
  }

  // Build a bargraph every 10 seconds
  int secmod = ss % 10;
  if (secmod) { // Show growing bar every 10 seconds
    ttgo->tft->fillRect(126 + secmod * 10, 215, 6, 15, TFT_ORANGE);
  } else {
    ttgo->tft->fillRoundRect(119, 210, 120, 29, 15, TFT_DARKCYAN);
  }
}
*/
