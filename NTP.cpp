#include "WProgram.h"
#include "NTP.h"

NTP::NTP(byte address[])
{
    _address[0] = address[0];
    _address[1] = address[1];
    _address[2] = address[2];
    _address[3] = address[3];
}

NTP::~NTP()
{
}

unsigned long NTP::get_time()
{
    call();
    return _send_timestamp;
}

int NTP::get_leap_indicator(byte b)
{
    return b >> 6;
}

int NTP::get_version(byte b)
{
    byte c = b << 2;
    return c >> 5;
}

int NTP::get_mode(byte b)
{
    byte c = b << 5;
    return c >> 5;
}

unsigned long NTP::get_ulong()
{
    unsigned long ulong = (unsigned long)UdpBytewise.read() << 24;
    ulong |= (unsigned long)UdpBytewise.read() << 16;
    ulong |= (unsigned long)UdpBytewise.read() << 8;
    ulong |= (unsigned long)UdpBytewise.read();
    return ulong;
}

unsigned long NTP::get_time_discard_precision()
{
    unsigned long time = get_ulong();
    // we are going to discard the sub-second stuff
    UdpBytewise.read();
    UdpBytewise.read();
    UdpBytewise.read();
    UdpBytewise.read();
    return time;
}

void NTP::write_n(int what, int how_many)
{
    for( int i = 0; i < how_many; i++ )
        UdpBytewise.write(what);
}

int NTP::send_ntp_packet()
{
    UdpBytewise.begin(123);
    UdpBytewise.beginPacket(_address, 123);
    // LI, Version, Mode
    UdpBytewise.write(B11100011);
    // Stratum
    UdpBytewise.write(0);
    // Polling Interval
    UdpBytewise.write(6);
    // Peer Clock Precision
    UdpBytewise.write(0xEC);
    // Root Delay
    write_n(0, 4);
    // Root Dispersion
    write_n(0, 4);
    // Reference Clock Id
    UdpBytewise.write(49);
    UdpBytewise.write(0x4E);
    UdpBytewise.write(49);
    UdpBytewise.write(52);
    // Reference CLock Update Time
    write_n(0, 8);
    // Originate Time Stamp
    write_n(0, 8);
    // Receive Time Stamp
    write_n(0, 8);
    // Transmit Time Stamp
    write_n(0, 8);
    // End
    return UdpBytewise.endPacket(); 
}

void NTP::call()
{
    send_ntp_packet();

    delay(1000);

    if ( UdpBytewise.available() ) {
        byte first_byte = UdpBytewise.read();
        _leap_indicator = get_leap_indicator(first_byte); 
        _version = get_version(first_byte);
        _mode = get_mode(first_byte);
        _stratum = (int)UdpBytewise.read();
        _polling_interval = 1 << ((int)UdpBytewise.read());
        _precision = (char)UdpBytewise.read();
        _delay_interval = (float) get_ulong();
        _dispersion = (float) get_ulong();

        _ref_clock_id[0] = UdpBytewise.read(); 
        _ref_clock_id[1] = UdpBytewise.read(); 
        _ref_clock_id[2] = UdpBytewise.read(); 
        _ref_clock_id[3] = UdpBytewise.read(); 

        _ref_clock_update_time = get_time_discard_precision();
        _orig_timestamp = get_time_discard_precision();
        _recv_timestamp = get_time_discard_precision();
        _send_timestamp = get_time_discard_precision();
    }
}
