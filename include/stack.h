/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : stack.h
 */

#ifndef __PLC_STACK_H
#define __PLC_STACK_H   1

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct stack_node {             /* структура узла стека */
        void              *data;
        struct stack_node *next;
} stack_node_s;

typedef struct thread_safe_stack {      /* структура стека с синхронизацией */
        stack_node_s    *top;
        pthread_mutex_t  mutex;
        size_t           size;
} thread_safe_stack_s;

/* инициализация стека */
void p_stack_init(thread_safe_stack_s *stack);

/* уничтожение стека */
void p_stack_destroy(thread_safe_stack_s *stack, void (*free_data)(void*));

/* добавление элемента в стек */
bool p_stack_push(thread_safe_stack_s *stack, void *data);

/* извлечение элемента из стека */
void *p_stack_pop(thread_safe_stack_s *stack);

/* просмотр верхнего элемента без извлечения */
void *p_stack_peek(thread_safe_stack_s *stack);

/* проверка на пустоту */
bool p_stack_empty(thread_safe_stack_s *stack);

/* получение размера стека */
size_t p_stack_size(thread_safe_stack_s *stack);

#endif  /* __PLC_STACK_H */
