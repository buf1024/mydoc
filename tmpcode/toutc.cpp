#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PSCF_CONV_1900_1970 2208988800LL

typedef struct tagTIMESTAMP_STRUCT
{
        int   year;
        int   month;
        int   day;
        int   hour;
        int   minute;
        int   second;
        unsigned int    fraction;
} TIMESTAMP_STRUCT;

long long timestampToUTCSec(TIMESTAMP_STRUCT& ts)
{
    struct tm tmTime;

    tmTime.tm_year = ts.year - 1900;
    tmTime.tm_mon = ts.month - 1;
    tmTime.tm_mday = ts.day;
    tmTime.tm_hour = ts.hour;
    tmTime.tm_min = ts.minute;
    tmTime.tm_sec = ts.second;

    return (mktime(&tmTime) + PSCF_CONV_1900_1970);
}

int main(int argc, char** argv)
{
    if(argc != 2) return -1;
    
    TIMESTAMP_STRUCT ts;
    sscanf(argv[1], "%d-%d-%d %d:%d:%d", &ts.year, &ts.month, &ts.day, &ts.hour, &ts.minute, &ts.second);
    printf("%lld", timestampToUTCSec(ts));
    return 0;
}
