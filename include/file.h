/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : file.h
 */

#ifndef __PLC_FILE_H
#define __PLC_FILE_H    1

int     p_file_copy(const char *f_src, const char *f_dst);
int     p_file_move(const char *f_src, const char *f_dst);

/* Функция включает один или более флагов состояния файла.
 * Флаги состояния файла:
 * O_RDONLY   - файл открыт только для чтения
 * O_WRONLY   - файл открыт только для записи
 * O_RDWR     - файл открыт для чтения и записи
 * O_EXEC     - файл открыт только для выполнения
 * O_SEARCH   - каталог открыт только для поиска
 * O_APPEND   - файл открыт для добавления в конец
 * O_NONBLOCK - неблокирующий режим
 * O_SYNC     - ожидать завершения операции записи (данных и атрибутов)
 * O_DSYNC    - ожидать завершения операции записи (только данных)
 * O_RSYNC    - синхронизировать операции чтения и записи
 * O_FSYNC    - ожидать завершения операции записи (только FreeBSD и Mac OS X)
 * O_ASYNC    - асинхронный режим ввода/вывода (только FreeBSD и Mac OS X)
 */
int     p_file_setfl(int fd, int flags);

#endif  /* __PLC_FILE_H */
