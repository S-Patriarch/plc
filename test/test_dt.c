#include <plc/dt.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
        char ds[11];
        char ts[9];
        char ds_convert[30];
        int res;

        res = getdate(ds, sizeof(ds));
        printf("get_date               : %s\n", ds);

        res = gettime(ts, sizeof(ts));
        printf("get_time               : %s\n", ts);

        res = date_convert(ds, sizeof(ds), ds_convert, sizeof(ds_convert), DCF_dBY);
        printf("date_convert - DCF_dBY : %s\n", ds_convert);
        res = date_convert(ds, sizeof(ds), ds_convert, sizeof(ds_convert), DCF_BdY);
        printf("date_convert - DCF_BdY : %s\n", ds_convert);

        exit(EXIT_SUCCESS);        
}

