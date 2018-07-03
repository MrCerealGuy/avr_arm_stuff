#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(15, 14, 19, 18, 17, 16);
int col = 0, row = 0; // position of cursor

// MAC and IP
byte mac[] = { 0xFF, 0x23, 0xBA, 0x7C, 0x5F, 0xDD };
IPAddress ip(192, 168, 1, 177);

// telnet defaults to port 23
EthernetServer server(23);
boolean gotAMessage = false; // whether or not you got a message from the client yet
int incomingByte = 0;

void setup()
{
    // set up the lcd's number of columns and rows:
    lcd.begin(16, 2);
    lcd.blink();

    // Default text to start with
    lcd.print("Running!");

    // initialize the ethernet device
    Ethernet.begin(mac, ip);

    // start listening for clients
    server.begin();

    // open the serial port
    Serial.begin(9600);
}

void loop()
{
    // wait for a new client:
    EthernetClient client = server.available();

    // when the client sends the first byte, say hello:
    if (client)
    {
        if (!gotAMessage)
        {
            Serial.println("We have a new client");
            client.println("Welcome to Andy's Telnet!");
            gotAMessage = true;
            lcd.clear();
        }

        lcd.setCursor(col, row);

        // read the bytes incoming from the client:
        char thisChar = client.read();

        if (thisChar >= 32 && thisChar <= 126)
        {
            // write the bytes to the lcd.
            lcd.write(thisChar);

            // echo back
            server.write(thisChar);

            // print the bytes to serial.
            Serial.print(thisChar);

            if (col < 15)
                col++;
            else
            {
                col = 0;

                if (row == 0)
                    row++;
                else
                    row = 0;
            }
        }
        else if (thisChar == 8) // backspace
        {
            if (col == 0)
            {
                if (row == 0)
                {
                } // do nothing
                else
                {
                    row = 0;
                    col = 15;
                }
            }
            else
            {
                col = col - 1;
            }

            lcd.setCursor(col, row);
            lcd.write(32); // overwrite with white space

            // clear char on telnet client, echo must be disabled
            server.write(8);
            server.write(32);
            server.write(8);

            lcd.setCursor(col, row);
        }
    }
}