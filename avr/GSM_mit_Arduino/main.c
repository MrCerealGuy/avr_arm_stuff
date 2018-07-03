#include "SIM900.h"
#include <SoftwareSerial.h>

char inSerial[50];
int i = 0;
boolean started = false;

void setup()
{
    // Serielle Schnittstelle initialisieren, Baudrate 9600
    Serial.begin(9600);
    Serial.println("- Andys Einstieg in die GSM Welt -\n");
    Serial.print("Teste ob Netz vorhanden...");

    // Versuche eine GSM-Verbindung aufzubauen...
    if (gsm.begin(4800))
    {
        Serial.println("ja ;-). Warte auf Befehle (AT commands): ");
        started = true;
        gsm.forceON();
    }
    else
        Serial.println("nein :-(");
};

// Befehle über die serielle Schnittstelle lesen bzw.
// empfangene Daten vom GSM-Modem ausgeben.
void loop()
{
    i = 0;

    if (Serial.available() > 0) // Wurden Daten über die serielle Schnittstelle gesendet?
    {
        while (Serial.available() > 0) // Ja, Daten zeichenweise einlesen
        {
            inSerial[i] = (Serial.read());
            delay(10);
            i++;
        }

        inSerial[i] = '\0'; // Kennzeichnung Stringende

        Serial.println(inSerial); // Befehl über serielle Schnittstelle ausgeben
        gsm.SimpleWriteln(inSerial); // Befehl ins GSM-Netz senden

        inSerial[0] = '\0'; // Befehl löschen
    }

    // Empfangene Daten aus dem GSM-Netz lesen (zeichenweise) und über serielle Schnittstelle
    // ausgeben
    gsm.SimpleRead();
};

Format!Style:
C++ online code formatter © 2014 by KrzaQ

Powered by vibe.d, the D language and clang-format