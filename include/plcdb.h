/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLCDB library.  */

/*
 *      Patriarch Library C DB:                         <plcdb.h>
 */

#ifndef __PLCDB_H
#define __PLCDB_H       1

#include <unistd.h>
#include <fcntl.h>      /* флаги для функции open и p_db_open  */
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>
#include <sys/uio.h>    /* struct iovec  */
#include <sys/stat.h>   /* struct stat  */
#include <plc/plcdef.h>
#include <plc/error.h>
#include <plc/file.h>

P_BEGIN_DECLS

typedef void * P_DBHANDLE_t;

/* Открывает или создает базу данных.  */
extern P_DBHANDLE_t p_db_open (const char *pathname, int oflag, ...) P_NOEXCEPT;

/* Закрывает доступ к базе данных.  */
extern void p_db_close (P_DBHANDLE_t h) P_NOEXCEPT;

/* Извлекает одну запись.  */
extern char *p_db_fetch (P_DBHANDLE_t h, const char *key) P_NOEXCEPT;

/* Сохраняет запись в базе данных.  */
extern int p_db_store (P_DBHANDLE_t h,
                       const char *key, const char *data, int flag) P_NOEXCEPT;

/* Удаляет заданную запись.  */
extern int p_db_delete (P_DBHANDLE_t h, const char *key) P_NOEXCEPT;

/* Переход к первой записи для функции p_db_nextrec.  */
extern void p_db_rewind (P_DBHANDLE_t h) P_NOEXCEPT;

/* Возвращает следующую запись.  */
extern char *p_db_nextrec (P_DBHANDLE_t h, char *key) P_NOEXCEPT;

/*
 * Флаги для функции p_db_store.
 */
#define P_DB_INSERT  1 /* вставить новую запись  */
#define P_DB_REPLACE 2 /* заменить существующую запись  */
#define P_DB_STORE   3 /* заменить или вставить  */

/*
 * Ограничения реализации.
 */
#define P_IDXLEN_MIN 6    /* ключ, разделитель, смещение, разделитель, длина, \n  */
#define P_IDXLEN_MAX 1024 /* выбрано произвольно  */
#define P_DATLEN_MIN 2    /* байт данных, \n  */
#define P_DATLEN_MAX 1024 /* выбрано произвольно  */

P_END_DECLS

#endif /* plcdb.h  */
