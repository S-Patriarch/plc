#include <plc/async.h>
#include <stdlib.h>
#include <stdbool.h>

future_s *p_async_create(void *(*func)(void*), void *arg) 
{
        future_s *future = (future_s*)malloc(sizeof(future_s));
        if (!future) return NULL;

        future->function  = func;
        future->argument  = arg;
        future->completed = false;
        future->result    = NULL;

        return future;
}

int p_async_launch(future_s *future) 
{
        return pthread_create(&future->thread, 
                              NULL, 
                              future->function, 
                              future->argument);
}

void *p_async_get(future_s *future) 
{
        if (!future->completed) {
                pthread_join(future->thread, &future->result);
                future->completed = true;
        }
        
        return future->result;
}

void p_async_free(future_s *future)
{
        if (!future) return;

        if (!future->completed)
                pthread_join(future->thread, NULL);
        
        free(future->result);
        free(future);
}

