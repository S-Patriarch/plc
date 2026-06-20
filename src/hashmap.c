/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            hashmap.c
 */

#include <plc/hashmap.h>

/* Константы FNV-1a для хеширования строк.  */
static const size_t FNV_BASE  = 0x811c9dc5;
static const size_t FNV_PRIME = 0x01000193;

/* Максимальный коэффициент загрузки (75%).  */
static const float LOAD_FACTOR_MAX = 0.75f;

/* Округляет вместимость до следующей степени двойки.  */
static size_t
p_next_power_of_two(size_t n) P_NOEXCEPT 
{
        if (n == 0)
                return(16); /* вместимость по умолчанию  */

        size_t power = 1;
        while (power < n)
                power <<= 1;

        return(power);
}

/* Хеш-функция FNV-1a для строки.  */
static size_t
p_hash_string(const char *s, size_t capacity) P_NOEXCEPT 
{
        size_t hash = FNV_BASE;

        while (*s) {
                hash ^= (unsigned char)*s++;
                hash *= FNV_PRIME;
        }

        /* capacity всегда степень двойки -> быстрый modulo через AND  */
        return(hash & (capacity - 1));
}

/* Поиск слота для ключа (используется в get/put/remove).  
   map - хеш-таблица;
   key - искомый ключ;
   insert_pos - если ключ не найден, сюда записывается позиция для вставки.
   Возвращает индекс  слота с ключем, или -1 если ключ не найден.  */
static ssize_t 
p_find_slot(struct hashmap_s *map,
            const char *key, size_t *insert_pos) P_NOEXCEPT 
{
        size_t capacity = map->capacity;
        size_t idx = p_hash_string(key, capacity);
        size_t first_deleted = capacity; /* позиция первого DELETED слота  */

        while (map->slots[idx].status != EMPTY) {
                if (map->slots[idx].status == OCCUPIED && 
                    strcmp(map->slots[idx].key, key) == 0) {
                        /* ключ найден  */
                        if (insert_pos)
                                *insert_pos = idx;
                        return(idx);
                }

                /* запоминаем первый удаленный слот для возможной вставки  */
                if (map->slots[idx].status == DELETED &&
                    first_deleted == capacity)
                        first_deleted = idx;

                /* линейное пробирование  */
                idx = (idx + 1) & (capacity - 1);
        }

        /* ключ не найден, возвращаем позицию для вставки  */
        if (insert_pos)
                *insert_pos = (first_deleted != capacity) ?
                              first_deleted               :
                              idx;
        return(-1);
}

/* Расширение хеш-таблицы при превышении порога загрузки.  */
static unsigned char
p_hashmap_resize(struct hashmap_s *map) P_NOEXCEPT 
{
        size_t i;
        size_t new_capacity = map->capacity * 2;
        size_t old_capacity = map->capacity;
        struct slot_s *old_slots = map->slots;

        /* создаем новую таблицу  */
        struct slot_s *new_slots = (struct slot_s *)calloc(new_capacity, 
                                   sizeof(struct slot_s));
        if (!new_slots)
                return(0);

        /* перехеширование всех элементов  */
        map->slots = new_slots;
        map->capacity = new_capacity;
        map->size = 0;
        map->threshold = (size_t)(new_capacity * LOAD_FACTOR_MAX);

        for (i = 0; i < old_capacity; i++) {
                if (old_slots[i].status == OCCUPIED) {
                        /* вставляем элемент в новую таблицу  */
                        size_t insert_pos;
                        p_find_slot(map, old_slots[i].key, &insert_pos);
                        map->slots[insert_pos].key = old_slots[i].key;
                        map->slots[insert_pos].value = old_slots[i].value;
                        map->slots[insert_pos].status = OCCUPIED;
                        map->size++;
                } else if (old_slots[i].status == DELETED) {
                        free(old_slots[i].key);
                }
        }

        free(old_slots);
        old_slots = NULL;
        return(1);
}

/* Создание новой хеш-таблицы.  */
struct hashmap_s *
p_hashmap_create(size_t init_capacity) P_NOEXCEPT 
{
        struct hashmap_s *map = (struct hashmap_s *)
                                malloc(sizeof(struct hashmap_s));
        if (!map) 
                return(NULL);

        size_t cap = p_next_power_of_two(init_capacity);
        map->capacity = cap;
        map->size = 0;
        map->threshold = (size_t)(cap * LOAD_FACTOR_MAX);
        map->slots = (struct slot_s *)calloc(cap, sizeof(struct slot_s));

        if (!map->slots) {
                free(map);
                map = NULL;
        }

        return(map);
}

/* Уничтожение хеш-таблицы.  */
void 
p_hashmap_destroy(struct hashmap_s **map) P_NOEXCEPT 
{
        size_t i;

        if (!map || !*map)
                return;

        struct hashmap_s *m = *map;
        for (i = 0; i < m->capacity; i++) {
                if (m->slots[i].status == OCCUPIED ||
                    m->slots[i].status == DELETED)
                        free(m->slots[i].key);
        }

        free(m->slots);
        free(m);
        *map = NULL;
}

/* Вставка или обновление элемента.  */
unsigned char 
p_hashmap_put(struct hashmap_s *map,
              const char *key, void *value) P_NOEXCEPT 
{
        size_t insert_pos;
        ssize_t existing;
        char *key_copy;

        if (!map || !key)
                return(0);

        /* расширение при необходимости  */
        if (map->size >= map->threshold)
                if (!p_hashmap_resize(map))
                        return(0);

        existing = p_find_slot(map, key, &insert_pos);
        if (existing != -1) {
                /* обновление существующего ключа  */
                map->slots[existing].value = value;
                return(1);
        }

        /* вставка нового ключа  */
        key_copy = strdup(key);
        if (!key_copy)
                return(0);

        /* если нашли позицию с DELETED, нужно освободить старый ключ  */
        if (map->slots[insert_pos].status == DELETED)
                free(map->slots[insert_pos].key);

        map->slots[insert_pos].key = key_copy;
        map->slots[insert_pos].value = value;
        map->slots[insert_pos].status = OCCUPIED;
        map->size++;

        return(1);
}

/* Получение элемента по ключу.  */
void *
p_hashmap_get(struct hashmap_s *map, const char *key) P_NOEXCEPT 
{
        ssize_t idx;

        if (!map || !key)
                return(NULL);

        idx = p_find_slot(map, key, NULL);
        if (idx != -1 && map->slots[idx].status == OCCUPIED)
                return(map->slots[idx].value);

        return(NULL);
}

/* Удаление элемента по ключу.  */
unsigned char 
p_hashmap_remove(struct hashmap_s *map, const char *key) P_NOEXCEPT 
{
        ssize_t idx;

        if (!map || !key)
                return(0);

        idx = p_find_slot(map, key, NULL);
        if (idx != -1 && map->slots[idx].status == OCCUPIED) {
                free(map->slots[idx].key);
                map->slots[idx].key = NULL;
                map->slots[idx].value = NULL;
                map->slots[idx].status = DELETED;
                map->size--;
                return(1);
        }

        return(0);
}

/* Проверка существования ключа.  */
unsigned char 
p_hashmap_contains(struct hashmap_s *map, const char *key) P_NOEXCEPT 
{
        return(p_hashmap_get(map, key) != NULL);
}

/* Возврат количества элементов.  */
size_t 
p_hashmap_size(struct hashmap_s *map) P_NOEXCEPT 
{
        return(map ? map->size : 0);
}

/* Возврат вместимости.  */
size_t 
p_hashmap_capacity(struct hashmap_s *map) P_NOEXCEPT 
{
        return(map ? map->capacity : 0);
}

/* Коэффициент загрузки.  */
float 
p_hashmap_load_factor(struct hashmap_s *map) P_NOEXCEPT 
{
        if (!map || map->capacity == 0)
                return(0.0f);
        
        return((float)map->size / map->capacity);
}

/* Очистка таблицы.  */
void 
p_hashmap_clear(struct hashmap_s *map) P_NOEXCEPT 
{
        size_t i;

        if (!map)
                return;

        for (i = 0; i < map->capacity; i++) {
                if (map->slots[i].status == OCCUPIED || 
                    map->slots[i].status == DELETED) {
                        free(map->slots[i].key);
                        map->slots[i].key = NULL;
                        map->slots[i].value = NULL;
                        map->slots[i].status = EMPTY;
                }
        }

        map->size = 0;
}

/* Инициализация итератора.  */
void 
p_hashmap_iter_init(struct hashmapiter_s *iter, struct hashmap_s *map) 
        P_NOEXCEPT 
{
        if (!iter || !map)
                return;

        iter->map = map;
        iter->current_index = 0;
}

/* Переход к следующему элементу.  */
unsigned char 
p_hashmap_iter_next(struct hashmapiter_s *iter, const char **key, void **value) 
        P_NOEXCEPT 
{
        if (!iter || !iter->map)
                return(0);

        struct hashmap_s *map = iter->map;
        while (iter->current_index < map->capacity) {
                if (map->slots[iter->current_index].status == OCCUPIED) {
                        if (key)
                                *key = map->slots[iter->current_index].key;
                        if (value)
                                *value = map->slots[iter->current_index].value;
                        iter->current_index++;
                        return(1);
                }
                iter->current_index++;
        }

        return(0);
}

