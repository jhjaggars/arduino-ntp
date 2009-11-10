#include <DateTime.h>
#include <DateTimeStrings.h>
#include <Ethernet.h>
#include <NTP.h>

#define RESYNC_TIME 60 * 15

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
byte ip[] = { 192, 168, 1, 44 };
byte gateway[] = { 192, 168, 1, 1 }; 

byte time_dot_nist_dot_gov[] = { 192, 43, 244, 18 };

NTP ntp(time_dot_nist_dot_gov);
unsigned long last_sync = 0;

void setup() 
{
    Serial.begin(9600);
    Ethernet.begin(mac,ip,gateway);
    sync();
}

void loop()
{
    sync();
    if( DateTime.available() )
    {
        digitalClockDisplay();
    }

    delay(1000);
}

void sync()
{
    unsigned long now = DateTime.now();
    if( (now - last_sync >= RESYNC_TIME) || last_sync == 0 )
    {
        DateTime.sync( ntp.get_unix_gmt() );
        last_sync = now;
        Serial.println("Synchronizing...");
    }
}

void digitalClockDisplay(){
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
