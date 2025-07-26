/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : stack.c
 */

#include <plc/stack.h>

void p_stack_init(thread_safe_stack_s *stack) 
{
        if (stack == NULL) return;

        stack->top = NULL;
        stack->size = 0;

        pthread_mutex_init(&stack->mutex, NULL);
}

void p_stack_destroy(thread_safe_stack_s *stack, void (*free_data)(void*)) 
{
        if (stack == NULL) return;

        pthread_mutex_lock(&stack->mutex);

        stack_node_s *current = stack->top;
        while (current != NULL) {
                stack_node_s *next = current->next;
                if (free_data != NULL)
                        free_data(current->data);
                free(current);
                current = next;
        }

        stack->top = NULL;
        stack->size = 0;

        pthread_mutex_unlock(&stack->mutex);
        pthread_mutex_destroy(&stack->mutex);
}

bool p_stack_push(thread_safe_stack_s *stack, void *data) 
{
        if (stack == NULL) return false;

       stack_node_s *new_node = (stack_node_s*)malloc(sizeof(stack_node_s));
       if (new_node == NULL) return false;

       new_node->data = data;

       pthread_mutex_lock(&stack->mutex);

       new_node->next = stack->top;
       stack->top = new_node;
       stack->size++;

       pthread_mutex_unlock(&stack->mutex);
       return true;
}

void *p_stack_pop(thread_safe_stack_s *stack) 
{
        if (stack == NULL) return NULL;

        pthread_mutex_lock(&stack->mutex);

        if (stack->top == NULL) {
                pthread_mutex_unlock(&stack->mutex);
                return NULL;
        }

        stack_node_s *node = stack->top;
        void *data = node->data;
        stack->top = node->next;
        stack->size--;

        pthread_mutex_unlock(&stack->mutex);

        free(node);
        return data;
}

void *p_stack_peek(thread_safe_stack_s *stack) 
{
        if (stack == NULL) return NULL;

        pthread_mutex_lock(&stack->mutex);

        void *data = (stack->top != NULL) ? stack->top->data : NULL;

        pthread_mutex_unlock(&stack->mutex);
        return data;
}

bool p_stack_empty(thread_safe_stack_s *stack) 
{
        if (stack == NULL) return true;

        pthread_mutex_lock(&stack->mutex);
        bool empty = (stack->top == NULL);
        pthread_mutex_unlock(&stack->mutex);

        return empty;
}

size_t p_stack_size(thread_safe_stack_s *stack) 
{
        if (stack == NULL) return 0;

        pthread_mutex_lock(&stack->mutex);
        size_t size = stack->size;
        pthread_mutex_unlock(&stack->mutex);

        return size;
}

