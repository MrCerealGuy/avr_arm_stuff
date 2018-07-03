#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#include "SoftwareSerial.h"
SoftwareSerial port(12, 13);
#else // Arduino 0022 - use modified NewSoftSerial
#include "WProgram.h"
#include "NewSoftSerial.h"
NewSoftSerial port(12, 13);
#endif

#include "EasyVR.h"
#include <LiquidCrystal.h>

EasyVR easyvr(port);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
int col = 0, row = 0; // position of cursor

// Groups and Commands
enum Groups
{
    GROUP_0 = 0,
    GROUP_1 = 1
};

enum Group0
{
    G0_COMPUTER = 0
};

enum Group1
{
    G1_NEXT = 0
};


EasyVRBridge bridge;

int8_t group, idx;

void setup()
{
    // bridge mode?
    if (bridge.check())
    {
        cli();
        bridge.loop(0, 1, 12, 13);
    }
    // run normally
    Serial.begin(9600);
    port.begin(9600);

    if (!easyvr.detect())
    {
        Serial.println("EasyVR not detected!");
        for (;;)
            ;
    }

    easyvr.setPinOutput(EasyVR::IO1, LOW);
    Serial.println("EasyVR detected!");
    easyvr.setTimeout(5);
    easyvr.setLanguage(3);

    group = EasyVR::TRIGGER; //<-- start group (customize)

    // set up the lcd's number of columns and rows:
    lcd.begin(16, 2);
    lcd.blink();

    // Default text to start with
    display("Running!");
    delay(1000);
}

void action();

void loop()
{
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

    Serial.print("Say a command in Group ");
    Serial.println(group);

    display("Say a command in Group " + String(group));

    easyvr.recognizeCommand(group);

    do
    {
        // can do some processing while waiting for a spoken command
    } while (!easyvr.hasFinished());

    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

    idx = easyvr.getCommand();

    if (idx >= 0)
    {
        // print debug message
        uint8_t train = 0;
        char name[32];
        Serial.print("Command: ");
        Serial.print(idx);

        if (easyvr.dumpCommand(group, idx, name, train))
        {
            Serial.print(" = ");
            Serial.println(name);

            display("Command: " + String(idx) + " = " + name);
            delay(1000);
        }
        else
            Serial.println();

        // perform some action
        action();
    }
    else // errors or timeout
    {
        if (easyvr.isTimeout())
        {
            Serial.println("Timed out, try again...");
            display("Timed out, try again...");
            delay(1000);
        }
        int16_t err = easyvr.getError();
        if (err >= 0)
        {
            Serial.print("Error ");
            Serial.println(err, HEX);

            String strErr;

            if (err == 17)
                strErr = "ERR_RECOG_FAIL";
            else if (err == 4)
                strErr = "ERR_DATACOL_TOO_SOFT";
            else
                strErr = "UNKNOWN";

            display("Error: " + strErr);
            delay(1000);
        }
    }
}

void action()
{
    switch (group)
    {
        case GROUP_0:
            switch (idx)
            {
                case G0_COMPUTER:
                    group = GROUP_1;
                    easyvr.playSound(1, EasyVR::VOL_FULL);
                    break;
            }
            break;

        case GROUP_1:
            switch (idx)
            {
                case G1_NEXT:
                    easyvr.playSound(0, EasyVR::VOL_FULL);
                    break;
            }
            break;
    }
}

void display(String strText)
{
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print(strText);

    if (strText.length() > 16)
    {
        lcd.setCursor(0, 1);
        lcd.print(strText.substring(16));
    }
}