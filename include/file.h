/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <file.h>
 */

#ifndef __PLC_FILE_H
#define __PLC_FILE_H    1

#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <plc/plcdef.h>
#include <plc/error.h>

P_BEGIN_DECLS

/* Функция включает один или более флагов состояния файла.
   Флаги состояния файла:
   O_RDONLY   - файл открыт только для чтения
   O_WRONLY   - файл открыт только для записи
   O_RDWR     - файл открыт для чтения и записи
   O_EXEC     - файл открыт только для выполнения
   O_SEARCH   - каталог открыт только для поиска
   O_APPEND   - файл открыт для добавления в конец
   O_NONBLOCK - неблокирующий режим
   O_SYNC     - ожидать завершения операции записи (данных и атрибутов)
   O_DSYNC    - ожидать завершения операции записи (только данных)
   O_RSYNC    - синхронизировать операции чтения и записи
   O_FSYNC    - ожидать завершения операции записи (только FreeBSD и Mac OS X)
   O_ASYNC    - асинхронный режим ввода/вывода (только FreeBSD и Mac OS X)  */
extern int p_file_setfl (int fd, int flags) P_NOEXCEPT;

extern int p_file_copy (const char *f_src, const char *f_dst) P_NOEXCEPT;
extern int p_file_move (const char *f_src, const char *f_dst) P_NOEXCEPT;

/* Функция проверки возможности наложения блокировки на участок файла.
   Если уже существует блокировка, которая может заблокировать выполнение
   запроса с заданными параметрами, эта функция возвращает идентификатор 
   процесса, владеющего блокировкой.  Иначе возвращается 0 (ложь).  Эту
   функцию желательно вызывать из следующих двух макросов.  */
extern pid_t p_lock_test (int fd, int type, off_t offset, int whence, 
                          off_t len) P_NOEXCEPT;

#define P_IS_READ_LOCKABLE(fd, offset, whence, len) \
                (p_lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define P_IS_WRITE_LOCKABLE(fd, offset, whence, len) \
                (p_lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

/* Функция наложения и снятия блокировки на участок файла.  
   Эта функция избавляет от необходимости всякий раз размещать и заполнять
   структуру flock.  Она выполняет все необходимые действия.  Поскольку в
   большинстве случаев эта функция будет вызываться для наложения или
   снятия блокировки (команда F_GETLK используется редко), желательно ее
   вызывать из следующих пяти макросов.  */
extern int p_lock_reg (int fd, int cmd, int type, off_t offset, int whence, 
                       off_t len) P_NOEXCEPT;

#define P_READ_LOCK(fd, offset, whence, len) \
                p_lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define P_READW_LOCK(fd, offset, whence, len) \
                p_lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define P_WRITE_LOCK(fd, offset, whence, len) \
                p_lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define P_WRITEW_LOCK(fd, offset, whence, len) \
                p_lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define P_UN_LOCK(fd, offset, whence, len) \
                p_lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

/* Установка блокировки для записи на весь файл.  */
extern int p_lock_file (int fd) P_NOEXCEPT;

#define P_LOCK_FILE(fd) \
                P_WRITE_LOCK((fd), 0, SEEK_SET, 0)

P_END_DECLS

#endif /* file.h  */
