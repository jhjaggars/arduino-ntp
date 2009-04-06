#include <Ethernet.h>
#include <NTP.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
byte ip[] = { 192, 168, 1, 44 };
byte gateway[] = { 192, 168, 1, 1 }; 

byte time_dot_nist_dot_gov[] = { 192, 43, 244, 18}; // time.nist.gov

void setup() 
{
    Serial.begin(9600);
    Ethernet.begin(mac,ip,gateway);
    NTP ntp(time_dot_nist_dot_gov);
    unsigned long time = ntp.get_gmt();    
    Serial.println(time);
}

void loop()
{
}

