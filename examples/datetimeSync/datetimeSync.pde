#include <DateTime.h>
#include <DateTimeStrings.h>
#include <Ethernet.h>
#include <NTP.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
byte ip[] = { 192, 168, 1, 44 };
byte gateway[] = { 192, 168, 1, 1 }; 

byte time_dot_nist_dot_gov[] = { 192, 43, 244, 18 };

void setup() 
{
    Serial.begin(9600);
    Ethernet.begin(mac,ip,gateway);
    NTP ntp(time_dot_nist_dot_gov);
    DateTime.sync( ntp.get_unix_gmt() );    
}

void loop()
{
    if( DateTime.available() )
    {
        digitalClockDisplay();
    }

    delay(1000);
}

void digitalClockDisplay() {
    // digital clock display of current date and time
    Serial.print(DateTimeStrings.dayStr(DateTime.DayofWeek));
    Serial.print(" ");
    Serial.print(DateTimeStrings.monthStr(DateTime.Month));
    Serial.print(" ");
    Serial.print(DateTime.Day,DEC);
    Serial.print(" ");
    Serial.print(DateTime.Hour,DEC);
    printDigits(DateTime.Minute);
    printDigits(DateTime.Second);
    Serial.println();
}

void printDigits(byte digits){
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits,DEC);
}
