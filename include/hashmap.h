/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <hashmap.h>
 */

#ifndef __PLC_HASHMAP_H
#define __PLC_HASHMAP_H         1

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <plc/plcdef.h>

/* Хеш-таблица с открытой адресацией (линейное пробирование).
   Хранит пары ключ-значение, где ключ - строка, значение - void *.  
   
   В качестве хеш-функции используется FNV-1a.
   FNV-1a - это простая и быстрая некриптографическая хеш-функция, созданная
   для хеш-таблиц.  Ее главная особенность - отличная скорость и равномерное
   распределение ключей при минимальных коллизиях.  

   Алгоритм работы хеш-функции заключается в том, что хеш вычисляется
   последовательно для каждого байта данных:
   1. Берется начальное значение (офсет) - например, 2166136261 для 32-битной
   версии.
   2. Для каждого байта данных:
        - FNV-1a: сначала хеш = хеш XOR байт, затем хеш = хеш * простое число
        FNV (например, 16777619 для 32 бит).
        - Классический FNV-1 делает наоборот: сначала умножение, потом XOR.
   Именно порядок XOR -> умножение в FNV-1a дает лучшее распределение для
   коротких строк и байтов с малыми значениями.

   Почему хороша для хеш-таблиц:
   - быстрая: только XOR и умножение, без ветвлений;
   - простая: легко реализовать, нет инициализации сложными константами;
   - хорошая лавинность: изменение одного бита входных данных меняет в среднем
   половину битов хеша;
   - детерминированная: одинаковый ключ всегда дает одинаковый хеш.

   Недостаток FNV-1a - не подходит для криптографии (легко подобрать коллизию
   или восстановить данные).  */

P_BEGIN_DECLS

/* Состояние слота (bucket).  */
enum slotstatus_e {
        EMPTY,          /* слот пуст и никогда не использовался  */
        OCCUPIED,       /* слот занят валидным элементом  */
        DELETED         /* слот был занят, но удален  */
};

/* Внутренняя структура слота.  */
struct slot_s {
        char *key;                /* ключ (динамически выделенная строка)  */
        void *value;              /* значение  */
        enum slotstatus_e status; /* состояние слота  */
};

/* Внутренняя структура хеш-таблицы.  */
struct hashmap_s {
        struct slot_s *slots;     /* массив слотов  */
        size_t size;              /* количество элементов  */
        size_t capacity;          /* вместимость (всегда степень двойки)  */
        size_t threshold;         /* порог для расширения  */
};

/* Итератор для обхода всех элементов.
   Пример использования:
        struct hashmapiter_s it;
        p_hashmap_iter_init(&it, map);
        const char *key;
        void *value;
        while (p_hashmap_iter_next(&it, &key, &value))
                printf("%s -> %p\n", key, value);  */
struct hashmapiter_s {
        struct hashmap_s *map;
        size_t current_index;
};

/* Создает новую хеш-таблицу с начальной вместимостью (емкость будет округлена 
   до степени двойки).
   init_capacity - желаемая начальная вместимость 
                   (0 = будет выбран по умолчанию).  */
extern struct hashmap_s *p_hashmap_create (size_t init_capacity) P_NOEXCEPT;

/* Уничтожает хеш-таблицу и освобождает всю память.
   map - указатель на указатель на hashmap_s (будет обнулен).  */
extern void p_hashmap_destroy (struct hashmap_s **map) P_NOEXCEPT;

/* Вставляет пару ключ-значение в таблицу (если ключ существует, значение
   перезаписывается).
   map - хеш-таблица;
   key - строка-ключ (будет скопирована);
   value - указатель на значение.
   Возвращает 1 (true) при успехе и 0 (false) при ошибке.  */
extern unsigned char p_hashmap_put (struct hashmap_s *map,
                                    const char *key, void *value) P_NOEXCEPT;

/* Возвращает значение по ключу или NULL, если ключ не найден.
   map - хеш-таблица;
   key - строка-ключ.
   Возвращает указатель на значение или NULL.  */
extern void *p_hashmap_get (struct hashmap_s *map, const char *key) P_NOEXCEPT;

/* Удаляет элемент по ключу.
   map - хеш-таблица;
   key - строка-ключ.
   Возвращает 1 (true) - элемент найден и удален,
   0 (false) - ключ не найден.  */
extern unsigned char p_hashmap_remove (struct hashmap_s *map, const char *key) 
        P_NOEXCEPT;

/* Проверяет существование ключа в таблице.
   map - хеш-таблица;
   key - строка-ключ.
   Возвращает 1 (true) - ключ существует, 0 (false) - не найден.  */
extern unsigned char p_hashmap_contains (struct hashmap_s *map, const char *key) 
        P_NOEXCEPT;

/* Возвращает количество элементов в таблице.  */
extern size_t p_hashmap_size (struct hashmap_s *map) P_NOEXCEPT;

/* Возвращает текущую вместимость таблицы.  */
extern size_t p_hashmap_capacity (struct hashmap_s *map) P_NOEXCEPT;

/* Возвращает коэффициент загрузки (size/capacity).  */
extern float p_hashmap_load_factor (struct hashmap_s *map) P_NOEXCEPT;

/* Очищает таблицу, удаляя все элементы.  */
extern void p_hashmap_clear (struct hashmap_s *map) P_NOEXCEPT;

/* Инициализирует итератор для перебора элементов.  */
extern void p_hashmap_iter_init (struct hashmapiter_s *iter, 
                                 struct hashmap_s *map) P_NOEXCEPT;

/* Переходит к следующему элементу.
   iter - итератор;
   key - выходной параметр для ключа;
   value - выходной параметр для значения;
   Возвращает 1 (true) - следующий элемент существует,
   0 (false) - обход закончен.  */
extern unsigned char p_hashmap_iter_next (struct hashmapiter_s *iter,
                                          const char **key, void **value) 
        P_NOEXCEPT;

P_END_DECLS

#endif /* hashmap.h  */
