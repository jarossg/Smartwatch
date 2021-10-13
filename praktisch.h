word ConvertRGB( byte R, byte G, byte B)
{
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

bool physischerKnopf()
{
  ttgo->power->readIRQ(); // This reads the interupts
  if(ttgo->power->isPEKShortPressIRQ())
  {
    ttgo->power->clearIRQ();
    return true;
  }
  else
  {
    ttgo->power->clearIRQ();
    return false;
  }
}

void warteAufKnopf()
{
    ttgo->power->readIRQ(); // This reads the interupts
    while(!ttgo->power->isPEKShortPressIRQ())
    {
      ttgo->power->clearIRQ();
      ttgo->power->readIRQ();
    }
    ttgo->power->clearIRQ();
}

void vibrieren(int Dauer) {
  digitalWrite(4, HIGH);
  delay(Dauer);
  digitalWrite(4, LOW);
}

void platzhalter()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    ttgo->tft->drawString("Platzhalter",  35, 30, 2);
    
    int16_t x, y;
    while (!ttgo->getTouch(x, y)) {} // Wait for touch
    while (ttgo->getTouch(x, y)) {}  // Wait for release to exit
}

void SetupWatch(void * parameter)
{
      // Turn on the IRQ used
  ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
  ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
  ttgo->power->clearIRQ();
  
  // Turn off unused power
  ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
  ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
  ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);  

    
    ttgo->tft->setTextFont(1);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
    //LVGL is not used, this line is not needed
    // ttgo->lvgl_begin();
}
