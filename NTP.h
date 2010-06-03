#ifndef NTP_h
#define NTP_h

#include "WProgram.h"
#include <UdpBytewise.h>

class NTP
{
    public:
        NTP(byte address[]);
        ~NTP();
        unsigned long get_gmt();
        unsigned long get_unix_gmt();
        unsigned long get_unix_tz(int offset);
        bool is_synced();
    private:
        bool successfully_synced;
        byte _address[4];
        int _leap_indicator;
        int _version;
        int _mode;
        int _stratum;
        int _polling_interval;
        int _precision;
        float _delay_interval;
        float _dispersion;
        byte _ref_clock_id[4];
        unsigned long _ref_clock_update_time;
        unsigned long _orig_timestamp;
        unsigned long _recv_timestamp;
        unsigned long _send_timestamp;

        int send_ntp_packet();
        unsigned long get_ulong();
        unsigned long get_time_discard_precision();
        void write_n(int what, int how_many);
        int get_leap_indicator(byte b);
        int get_version(byte b);
        int get_mode(byte b);
        void call();
};

#endif
