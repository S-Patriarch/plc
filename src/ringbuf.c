/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            ringbuf.c
 */

#include <plc/ringbuf.h>

/* Создание буфера под элементы любого типа.  */
struct ringbuf_s *
p_ringbuf_create(size_t esize, size_t capacity) P_NOEXCEPT 
{
        struct ringbuf_s *rb = 
                (struct ringbuf_s *)malloc(sizeof(struct ringbuf_s));

        rb->rb_data = malloc(esize * capacity);
        rb->rb_esize = esize;
        rb->rb_capacity = capacity;
        rb->rb_head = rb->rb_tail = rb->rb_count = 0;

        return rb;
}

/* Запись элемента (копирует данные в буфер).  */
unsigned char 
p_ringbuf_push(struct ringbuf_s *rb, const void *value) P_NOEXCEPT 
{
        if (rb->rb_count == rb->rb_capacity)
                return 0;

        char *target = (char *)rb->rb_data + rb->rb_tail * rb->rb_esize;
        memcpy(target, value, rb->rb_esize);

        rb->rb_tail = (rb->rb_tail + 1) % rb->rb_capacity;
        rb->rb_count++;

        return 1;
}

/* Чтение элемента (копирует данные наружу).  */
unsigned char 
p_ringbuf_pop(struct ringbuf_s *rb, void *out) P_NOEXCEPT 
{
        if (rb->rb_count == 0)
                return 0;

        char *src = (char *)rb->rb_data + rb->rb_head * rb->rb_esize;
        memcpy(out, src, rb->rb_esize);

        rb->rb_head = (rb->rb_head + 1) % rb->rb_capacity;
        rb->rb_count--;

        return 1;
}

/* Прямой доступ к элементу (без копирования).  */
void *
p_ringbuf_get(struct ringbuf_s *rb, size_t index) P_NOEXCEPT 
{
        if (index >= rb->rb_count)
                return NULL;

        size_t pos = (rb->rb_head + index) % rb->rb_capacity;

        return (char *)rb->rb_data + pos * rb->rb_esize;
}

/* Пакетная запись.  */
size_t 
p_ringbuf_push_many(struct ringbuf_s *rb, const void *src, 
                    size_t count) P_NOEXCEPT 
{
        size_t available = rb->rb_capacity - rb->rb_count;
        size_t to_write = (count < available) ? count : available;

        size_t first_chunk = rb->rb_capacity - rb->rb_tail;
        char *dst = (char *)rb->rb_data + rb->rb_tail * rb->rb_esize;

        if (to_write <= first_chunk) {
                memcpy(dst, src, to_write * rb->rb_esize);
        } else {
                memcpy(dst, src, first_chunk * rb->rb_esize);
                memcpy(rb->rb_data, (char *)src + first_chunk * rb->rb_esize, 
                       (to_write - first_chunk) * rb->rb_esize);
        }

        rb->rb_tail = (rb->rb_tail + to_write) % rb->rb_capacity;
        rb->rb_count += to_write;

        return to_write; /* сколько реально записали  */
}

/* Пакетное чтение.  */
size_t 
p_ringbuf_pop_many(struct ringbuf_s *rb, void *dst, 
                   size_t count) P_NOEXCEPT 
{
        size_t to_read = (count < rb->rb_count) ? count : rb->rb_count;

        size_t first_chunk = rb->rb_capacity - rb->rb_head;
        char *src = (char *)rb->rb_data + rb->rb_head * rb->rb_esize;

        if (to_read <= first_chunk) {
                memcpy(dst, src, to_read * rb->rb_esize);
        } else {
                memcpy(dst, src, first_chunk * rb->rb_esize);
                memcpy((char *)dst + first_chunk * rb->rb_esize, rb->rb_data, 
                       (to_read - first_chunk) * rb->rb_esize);
        }

        rb->rb_head = (rb->rb_head + to_read) % rb->rb_capacity;
        rb->rb_count -= to_read;

        return to_read; /* сколько реально прочитали  */
}

