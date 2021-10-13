// https://www.instructables.com/id/Lilygo-T-Watch-2020-Arduino-Framework/

#include "config.h"
#include <soc/rtc.h>

TTGOClass *ttgo;

#include "WLAN.h"
#include "BLE.h"

#include "praktisch.h"
#include "Apps.h"
#include "NTP.h"
#include "Zeit.h"
#include "WebConfig.h"
#include "Toroffner.h"


uint32_t targetTime = 0;       // for next 1 second display update
// uint32_t clockUpTime = 0;      // track the time the clock is displayed


#include <time.h>

TaskHandle_t Core0TaskHnd;

void setup()
{
    Serial.begin(9600);
    
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

  xTaskCreatePinnedToCore(SetupWatch,"CPU_0", 1000,NULL,&Core0TaskHnd,0);



    //Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();

    //Synchronize time to system time
    ttgo->rtc->syncToSystem();

    displayTime(true); // Our GUI to show the time
    ttgo->openBL(); // Turn on the backlight
    ttgo->setBrightness(16); //Helligkeit in 8 Bit
    pinMode(4, OUTPUT); //Motor für Vibration

    //setupBLE();
 
}

void loop()
{

    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        displayTime(ss == 0); // Call every second but only update time every minute
    }

      int16_t x, y;
    if (ttgo->getTouch(x, y)) {
        while (ttgo->getTouch(x, y)) {} // wait for user to release

        // This is where the app selected from the menu is launched
        // If you add an app, follow the variable update instructions
        // at the beginning of the menu code and then add a case
        // statement on to this switch to call your paticular
        // app routine.

        switch (modeMenu()) { // Call modeMenu. The return is the desired app number
        case 0: // Zero is the clock, just exit the switch
            break;
        case 1:
            jSats();
            break;
        case 2:
            appAccel();
            break;
        case 3:
            appBattery();
            break;
        case 4:
            appTouch();
            break;
        case 5:
            platzhalter();
            break;
        case 6:
            TestApp();
            break;
        case 7:
            NTP();
            break;
        case 8:
            Uhrstellen();
            break; 
        case 9:
            WebConfig();
            break;
        case 10:
            Toroffner();
            break;                
        }
        displayTime(true);
    }
}
