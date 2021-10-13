

// The basic Time Display GUI
// if you are just updating the colon, fullUpdate =0
// if you want to update the complete display, fullUpdate =1
// This helps reduce flicker

const int maxApp = 11; // number of apps
String appName[maxApp] = {"Uhr", "Jupiter", "Gyro", "Batterie", "Touch", "Platzhalter", "Test", "NTP", "Uhr stellen", "WebConfig", "Toroeffner"}; // app names



void TestApp()
{
    //vibrieren(500);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    ttgo->tft->drawString("TestApp",  35, 30, 2);
    
    int16_t x, y;
    //while (!ttgo->getTouch(x, y)) {} // Wait for touch
    //while (ttgo->getTouch(x, y)) {}  // Wait for release to exit

    warteAufKnopf();
    ttgo->tft->fillScreen(TFT_BLACK);
}

int getTnum();
void prtTime(byte digit);
void setMenuDisplay(int mSel);

byte xcolon = 0; // location of the colon




// Show the accelerometer working

void appAccel()
{
    ttgo->bma->begin();
    ttgo->bma->enableAccel();
    ttgo->tft->fillScreen(TFT_BLACK);
    int16_t x, y;
    int16_t xpos, ypos;

    Accel acc;

    while (!ttgo->getTouch(x, y)) { // Wait for touch to exit

        ttgo->bma->getAccel(acc);
        xpos = acc.x;
        ypos = acc.y;
        ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_RED); // draw dot
        delay(100);
        ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_BLACK); // erase previous dot
    }

    while (ttgo->getTouch(x, y)) {}  // Wait for release to return to the clock

    ttgo->tft->fillScreen(TFT_BLACK); // Clear screen
}

uint8_t modeMenu()
{
    int mSelect = 0; // The currently highlighted app
    int16_t x, y;

    boolean exitMenu = false; // used to stay in the menu until user selects app

    setMenuDisplay(0); // display the list of Apps

    while (!exitMenu) {
        if (ttgo->getTouch(x, y)) { // If you have touched something...

            while (ttgo->getTouch(x, y)) {} // wait until you stop touching

            if (y >= 160) { // you want the menu list shifted up
                mSelect += 1;
                if (mSelect == maxApp) mSelect = 0;
                setMenuDisplay(mSelect);
            }

            if (y <= 80) { // you want the menu list shifted down
                mSelect -= 1;
                if (mSelect < 0) mSelect = maxApp - 1;
                setMenuDisplay(mSelect);
            }
            if (y > 80 && y < 160) { // You selected the middle
                exitMenu = true;
            }
        }

    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;

}


void setMenuDisplay(int mSel)
{

    int curSel = 0;
    // Display mode header
    ttgo->tft->fillScreen(ConvertRGB(0,184,49));
    ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);

    // Display apps
    if (mSel == 0) curSel = maxApp - 1;
    else curSel = mSel - 1;

    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 30);
    ttgo->tft->println(appName[curSel]);

    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(appName[mSel]);

    if (mSel == maxApp - 1) curSel = 0;
    else curSel = mSel + 1;

    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(appName[curSel]);
}

// Check out the touch screen
// Will display the x and y coordinates of where you touch
// for 10 seconds and then return to time

void appTouch()
{
    uint32_t endTime = millis() + 10000; // Timeout at 10 seconds
    int16_t x, y;
    ttgo->tft->fillScreen(TFT_BLACK);

    while (endTime > millis()) {
        ttgo->getTouch(x, y);
        ttgo->tft->fillRect(98, 100, 70, 85, TFT_BLACK);
        ttgo->tft->setCursor(80, 100);
        ttgo->tft->print("X:");
        ttgo->tft->println(x);
        ttgo->tft->setCursor(80, 130);
        ttgo->tft->print("Y:");
        ttgo->tft->println(y);
        delay(25);
    }

    while (ttgo->getTouch(x, y)) {}  // Wait for release to exit
    ttgo->tft->fillScreen(TFT_BLACK);
}

#define APP_TIME_ZONE   -4  // I am East Coast in Daylight Savings

void jSats()
{
    // Get the current info
    RTC_Date tnow = ttgo->rtc->getDateTime();

    uint8_t hh = tnow.hour;
    uint8_t mm = tnow.minute;
    uint8_t ss = tnow.second;
    uint8_t dday = tnow.day;
    uint8_t mmonth = tnow.month;
    uint16_t yyear = tnow.year;

    float tDay = dday; // Calculate the day plus fractional day
    tDay += (hh - APP_TIME_ZONE) / 24.0;
    tDay += mm / 1440.0;
    int16_t tYear = yyear;
    int8_t tMonth = mmonth;

    int16_t cYear, cMonth;
    if (tMonth < 3) {
        cYear = tYear - 1;
        cMonth = tMonth + 12;
    } else {
        cYear = tYear;
        cMonth = tMonth;
    }
    // Calculate the Julian Date offset from Epoch
    int a = cYear / 100;
    int b = 2 - a + (int)(a / 4);
    long c = 365.25 * cYear;
    long d = 30.6001 * (cMonth + 1);
    float N = b + c + d + tDay - 694025.5;

    // Calc moon positions
    float P = PI / 180;
    float MT = (358.476 + 0.9856003 * N) * P;
    float MJ = (225.328 + 0.0830853 * N) * P;
    float JJ = 221.647 + 0.9025179 * N;
    float VT = 1.92 * sin(MT) + 0.02 * sin(2 * MT);
    float VJ = 5.55 * sin(MJ) + 0.17 * sin(2 * MJ);
    float K = (JJ + VT - VJ) * P;
    float DT = sqrt(28.07 - 10.406 * cos(K));
    float Z1 = sin(K) / DT;
    float I = atan(Z1 / sqrt(1 - Z1 * Z1));
    I = I / P;
    float F = (N - DT / 173);
    float F1 = I - VJ;
    float U1 = 84.5506 + 203.405863 * F + F1;
    float U2 = 41.5015 + 101.2916323 * F + F1;
    float U3 = 109.9770 + 50.2345169 * F + F1;
    float U4 = 176.3586 + 21.4879802 * F + F1;
    float X1 = 5.906 * sin(U1 * P);
    float X2 = 9.397 * sin(U2 * P);
    float X3 = 14.989 * sin(U3 * P);
    float X4 = 26.364 * sin(U4 * P);
    // Print out results

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor( 0, 10);
    ttgo->tft->setTextColor(TFT_ORANGE);
    ttgo->tft->print(" IO: ");
    ttgo->tft->print(X1, 1);
    ttgo->tft->setCursor( 0, 30);
    ttgo->tft->setTextColor(TFT_BLUE);
    ttgo->tft->print(" EU: ");
    ttgo->tft->println(X2, 1);
    ttgo->tft->setCursor( 0, 50);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->print(" GA: ");
    ttgo->tft->println(X3, 1);
    ttgo->tft->setCursor( 0, 70);
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->print(" CA: ");
    ttgo->tft->println(X4, 1);

    //Now display them as they would appear

    ttgo->tft->fillCircle(119, 155, 6, TFT_RED); // Jupiter

    ttgo->tft->setTextColor(TFT_ORANGE);
    ttgo->tft->fillCircle(int(X1 * 4 + 119), 155, 2, TFT_ORANGE);
    ttgo->tft->drawString("I", int(X1 * 4 + 119) - 3, 175, 1);
    ttgo->tft->setTextColor(TFT_BLUE);
    ttgo->tft->fillCircle(int(X2 * 4 + 119), 155, 2, TFT_BLUE);
    ttgo->tft->drawString("E", int(X2 * 4 + 119) - 3, 175, 1);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->fillCircle(int(X3 * 4 + 119), 155, 2, TFT_GREEN);
    ttgo->tft->drawString("G", int(X3 * 4 + 119) - 3, 175, 1);
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->fillCircle(int(X4 * 4 + 119), 155, 2, TFT_YELLOW);
    ttgo->tft->drawString("C", int(X4 * 4 + 119) - 3, 175, 1);


    // Wait for touch to return
    int16_t x, y;
    while (!ttgo->getTouch(x, y)) {} // Wait for touch
    while (ttgo->getTouch(x, y)) {}  // Wait for release
    ttgo->tft->fillScreen(TFT_BLACK);
}

// Display Battery Data

void appBattery()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    ttgo->tft->drawString("BATT STATS",  35, 30, 2);
    ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);

    // Turn on the battery adc to read the values
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    // get the values
    float vbus_v = ttgo->power->getVbusVoltage();
    float vbus_c = ttgo->power->getVbusCurrent();
    float batt_v = ttgo->power->getBattVoltage();
    int per = ttgo->power->getBattPercentage();

// Print the values
    ttgo->tft->setCursor(0, 100);
    ttgo->tft->print("Vbus: "); ttgo->tft->print(vbus_v); ttgo->tft->println(" mV");
    ttgo->tft->setCursor(0, 130);
    ttgo->tft->print("Vbus: "); ttgo->tft->print(vbus_c); ttgo->tft->println(" mA");
    ttgo->tft->setCursor(0, 160);
    ttgo->tft->print("BATT: "); ttgo->tft->print(batt_v); ttgo->tft->println(" mV");
    ttgo->tft->setCursor(0, 190);
    ttgo->tft->print("Per: "); ttgo->tft->print(per); ttgo->tft->println(" %");

    int16_t x, y;
    while (!ttgo->getTouch(x, y)) {} // Wait for touch
    while (ttgo->getTouch(x, y)) {}  // Wait for release to exit
    //Clear screen
    ttgo->tft->fillScreen(TFT_BLACK);
}
