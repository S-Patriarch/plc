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
#include <plc/plcdef.h>

typedef struct stack_node {        /* структура узла стека  */
        void              *data;
        struct stack_node *next;
} stack_node_s;

typedef struct thread_safe_stack { /* структура стека с синхронизацией  */
        stack_node_s    *top;
        pthread_mutex_t  mutex;
        size_t           size;
} thread_safe_stack_s;

/* инициализация стека  */
void    p_stack_init(thread_safe_stack_s *stack) P_NOEXCEPT;

/* уничтожение стека  */
void    p_stack_destroy(thread_safe_stack_s *stack, void (*free_data)(void*)) P_NOEXCEPT;

/* добавление элемента в стек  */
bool    p_stack_push(thread_safe_stack_s *stack, void *data) P_NOEXCEPT;

/* извлечение элемента из стека  */
void   *p_stack_pop(thread_safe_stack_s *stack) P_NOEXCEPT;

/* просмотр верхнего элемента без извлечения  */
void   *p_stack_peek(thread_safe_stack_s *stack) P_NOEXCEPT;

/* проверка на пустоту  */
bool    p_stack_empty(thread_safe_stack_s *stack) P_NOEXCEPT;

/* получение размера стека  */
size_t  p_stack_size(thread_safe_stack_s *stack) P_NOEXCEPT;

#endif  /* __PLC_STACK_H  */
