#include <stdlib.h>
#include <stdbool.h>
#include <plc/async.h>

struct future_s *
p_async_create(void *(*func)(void*), void *arg) P_NOEXCEPT 
{
        struct future_s *future = 
                (struct future_s*)malloc(sizeof(struct future_s));
        if (!future) return (NULL);

        future->function  = func;
        future->argument  = arg;
        future->completed = false;
        future->result    = NULL;

        return (future);
}

int 
p_async_launch(struct future_s *future) P_NOEXCEPT
{
        return (pthread_create(&future->thread, 
                               NULL, 
                               future->function, 
                               future->argument));
}

void *
p_async_get(struct future_s *future) P_NOEXCEPT
{
        if (!future->completed) {
                pthread_join(future->thread, &future->result);
                future->completed = true;
        }
        
        return (future->result);
}

void 
p_async_free(struct future_s *future) P_NOEXCEPT
{
        if (!future) return;

        if (!future->completed)
                pthread_join(future->thread, NULL);
        
        free(future->result);
        free(future);
}

