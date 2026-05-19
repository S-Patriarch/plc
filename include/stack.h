/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <stack.h>
 */

#ifndef __PLC_STACK_H
#define __PLC_STACK_H   1

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

struct stack_node_s { /* структура узла стека  */
        void                    *data;
        struct stack_node_s     *next;
};

struct thread_safe_stack_s { /* структура стека с синхронизацией  */
        struct stack_node_s     *top;
        pthread_mutex_t         mutex;
        size_t                  size;
};

/* Инициализация стека.  */
extern void p_stack_init (struct thread_safe_stack_s *stack) P_NOEXCEPT;

/* Уничтожение стека.  */
extern void p_stack_destroy (struct thread_safe_stack_s *stack, 
                             void (*free_data)(void*)) P_NOEXCEPT;

/* Добавление элемента в стек.  */
extern bool p_stack_push (struct thread_safe_stack_s *stack, void *data) P_NOEXCEPT;

/* Извлечение элемента из стека.  */
extern void *p_stack_pop (struct thread_safe_stack_s *stack) P_NOEXCEPT;

/* Просмотр верхнего элемента без извлечения.  */
extern void *p_stack_peek (struct thread_safe_stack_s *stack) P_NOEXCEPT;

/* Проверка на пустоту.  */
extern bool p_stack_empty (struct thread_safe_stack_s *stack) P_NOEXCEPT;

/* Получение размера стека.  */
extern size_t p_stack_size (struct thread_safe_stack_s *stack) P_NOEXCEPT;

P_END_DECLS

#endif /* stack.h  */
