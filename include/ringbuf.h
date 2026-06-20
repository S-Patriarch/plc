/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <ringbuf.h>
 */

#ifndef __PLC_RINGBUF_H
#define __PLC_RINGBUF_H         1

/* Кольцевой буфер (ringbuf).
   Это реализация очереди (queue) через фиксированный массив и два целых 
   индекса.  */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

struct ringbuf_s {
        void   *rb_data;        /* указатель на сырую память  */
        size_t  rb_esize;       /* размер одного элемента (sizeof(T))  */
        size_t  rb_capacity;    /* максимальное количество элементов  */
        size_t  rb_head;        /* индекс чтения (голова)  */
        size_t  rb_tail;        /* индекс записи (хвост)  */
        size_t  rb_count;       /* количество элементов  */
};

extern struct ringbuf_s *p_ringbuf_create (size_t esize, size_t capacity) 
        P_NOEXCEPT;
extern unsigned char p_ringbuf_push (struct ringbuf_s *rb, const void *value) 
        P_NOEXCEPT;
extern unsigned char p_ringbuf_pop (struct ringbuf_s *rb, void *out) 
        P_NOEXCEPT;
extern void *p_ringbuf_get (struct ringbuf_s *rb, size_t index) 
        P_NOEXCEPT;

/* Функции p_ringbuf_push_many и p_ringbuf_pop_many являются batch-операциями.
   Batch-операция - это функция, которая за один вызов обрабатывает несколько
   элементов буфера вместо одного. 
   Пример:
        // Медлено: 1000 вызовов pop
        for (int i = 0; i < 1000; i++)
                p_ringbuf_pop(rb, &out[i]);
        // Быстро: 1 вызов pop_many
        int count = p_ringbuf_pop_many(rb, out, 1000);  */

extern size_t p_ringbuf_push_many (struct ringbuf_s *rb, const void *src, 
                                   size_t count) P_NOEXCEPT;
extern size_t p_ringbuf_pop_many (struct ringbuf_s *rb, void *dst, 
                                  size_t count) P_NOEXCEPT;

P_END_DECLS

#endif /* ringbuf.h  */
