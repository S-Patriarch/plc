/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            thread.c
 */

#include <plc/thread.h>

int 
p_thread_detach(void *(*fn)(void *), void *arg) P_NOEXCEPT 
{
        int             err;
        pthread_t       tid;
        pthread_attr_t  attr;

        err = pthread_attr_init(&attr);
        if (err != 0)
                return(err);

        err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if (err == 0)
                err = pthread_create(&tid, &attr, fn, arg);

        pthread_attr_destroy(&attr);
        return(err);
}

