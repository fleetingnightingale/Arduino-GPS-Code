
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GPS.h>

File myFile;

//Name of the hardware serial port
#define GPSSerial Serial1

//connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

uint32_t timer = millis();



void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  GPSSerial.println(PMTK_Q_RELEASE);

}

void loop() {
  char c = GPS.read();
  if (GPSECHO)
    if (c) Serial.print(c);
  if (GPS.newNMEAreceived())  {
    Serial.println(GPS.lastNMEA());
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }

  if (millis() - timer > 2000) {
    timer = millis();
    myFile = SD.open("GPS.txt",FILE_WRITE);
    myFile.print("\nTime: ");
    if (GPS.hour < 10) {myFile.print('0'); }
    myFile.print(GPS.hour, DEC); myFile.print(':');
    if (GPS.minute < 10) { myFile.print('0'); }
    myFile.print(GPS.minute, DEC); myFile.print(':');
    if (GPS.seconds < 10) { myFile.print('0'); }
    myFile.print(GPS.seconds, DEC); myFile.print('.');
    if (GPS.milliseconds < 10) {
      myFile.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      myFile.print("0");
      myFile.close();
    }
    }
  }
