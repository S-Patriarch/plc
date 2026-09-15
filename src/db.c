/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLCDB library.  */

/*
 *      Patriarch Library C DB:                         db.c
 */

#include <plc/plcdb.h>

/*
 * Внутренние константы, имеющие отношение к индексному файлу.
 * Они используются при создании записей в индексном файле и в
 * файле с данными.
 */
#define P_IDXLEN_SZ 4 /* размер поля длины индексной записи
                         (в символах ASCII)  */

/*
 * Следующие определения необходимы для работы с цепочками в
 * хеш-таблице м в списке свободных записей в индексном файле.
 */
#define P_PTR_SZ    7        /* размер поля-указателя в цепочке  */
#define P_PTR_MAX   999999   /* максимальное смещение
                                в файле = 10**P_PTR_SZ - 1  */
#define P_NHASH_DEF 137      /* размер хеш-таблицы по умолчанию  */
#define P_FREE_OFF  0        /* начало списка свободных записей в
                                индексном файле  */
#define P_HASH_OFF  P_PTR_SZ /* начало хеш-таблицы в индексном файле  */

typedef unsigned long   P_DBHASH_t; /* значение хешей  */
typedef unsigned long   P_COUNT_t;  /* беззнаковый счетчик  */

/*
 * Веутреннее представление базы данных в библиотеке.
 */
typedef struct {
        int        idxfd;        /* дескриптор индексного файла  */
        int        datfd;        /* дескриптор файла с данными  */
        char      *idxbuf;       /* адрес буфера для индексной записи  */
        char      *datbuf;       /* адрес буфера для записи с данными  */
        char      *name;         /* имя базы данных, под которым она была открыта  */
        off_t      idxoff;       /* смещение индексной записи в индексном файле, ключ
                                    начинается с позиции (idxoff + P_PTR_SZ + P_IDXLEN_SZ)  */
        size_t     idxlen;       /* длина индексной записи
                                    кроме P_IDXLEN_SZ байт в начале записи
                                    включая символ перевода строки в конце записи  */
        off_t      datoff;       /* смещение записи с данными в файле данных  */
        size_t     datlen;       /* длина записи с данными
                                    включая символ перевода строки в конце записи  */
        off_t      ptrval;       /* содержимое указателя на цепочку в индексной записи  */
        off_t      ptroff;       /* смещение указателя, содержащего адрес этой записи  */
        off_t      chainoff;     /* смещение цепочки для этой индексной записи  */
        off_t      hashoff;      /* смещение хеш-таблицы в индексном файле  */
        P_DBHASH_t nhash;        /* текущий размер хеш-таблицы  */
        P_COUNT_t  cnt_delok;    /* счетчик удачных операций удаления  */
        P_COUNT_t  cnt_delerr;   /* счетчик ошибочных операций удаления  */
        P_COUNT_t  cnt_fetchok;  /* счетчик удачных операций извлечения данных  */
        P_COUNT_t  cnt_fetcherr; /* счетчик ошибочных операций извлечения данных  */
        P_COUNT_t  cnt_nextrec;  /* nextrec  */
        P_COUNT_t  cnt_stor1;    /* store: P_DB_INSERT, нет пустых записей, добавить  */
        P_COUNT_t  cnt_stor2;    /* store: P_DB_INSERT, есть пустые записи,
                                    использовать их  */
        P_COUNT_t  cnt_stor3;    /* store: P_DB_REPLACE, другая длина, добавить  */
        P_COUNT_t  cnt_stor4;    /* store: P_DB_REPLACE, та же длина, перезаписать  */
        P_COUNT_t  cnt_storerr;  /* счетчик ошибок добавления записи  */
} DB_s;

/*
 * Внутренние функции.
 */
static DB_s       *_db_alloc (int) P_NOEXCEPT;
static void        _db_dodelete (DB_s *) P_NOEXCEPT;
static int         _db_find_and_lock (DB_s *, const char *, int) P_NOEXCEPT;
static int         _db_findfree (DB_s *, int, int) P_NOEXCEPT;
static void        _db_free (DB_s *) P_NOEXCEPT;
static P_DBHASH_t  _db_hash (DB_s *, const char *) P_NOEXCEPT;
static char       *_db_readdat (DB_s *) P_NOEXCEPT;
static off_t       _db_readidx (DB_s *, off_t) P_NOEXCEPT;
static off_t       _db_readptr (DB_s *, off_t) P_NOEXCEPT;
static void        _db_writedat (DB_s *, const char *, off_t, int) P_NOEXCEPT;
static void        _db_writeidx (DB_s *, const char *, off_t, int, off_t) P_NOEXCEPT;
static void        _db_writeptr (DB_s *, off_t, off_t) P_NOEXCEPT;

/*
 * Открывает или создает базу данных.
 * Аргументы аналогичны функции open.
 */
P_DBHANDLE_t
p_db_open(const char *pathname, int oflag, ...) P_NOEXCEPT
{
        DB_s            *db;
        int             len, mode;
        size_t          i;
        char            asciiptr[P_PTR_SZ + 1],
                        hash[(P_NHASH_DEF + 1) * P_PTR_SZ + 2];
                        /* +2 для символа перевода строки и нулевого символа  */
        struct stat     statbuff;

        /* Разместить в памяти структуру DB_s и все необходимые буферы.  */
        len = strlen(pathname);
        if ((db = _db_alloc(len)) == NULL)
                p_error_dump("p_db_open: ошибка размещения структуры DB_s");
        db->nhash = P_NHASH_DEF;  /* размер таблицы хешей  */
        db->hashoff = P_HASH_OFF; /* начало хеш-таблицы в индексном файле  */
        strcpy(db->name, pathname);
        strcat(db->name, ".idx");

        if (oflag & O_CREAT) {
                va_list ap;

                va_start(ap, oflag);
                mode = va_arg(ap, int);
                va_end(ap);

                /* Создать индексный файл и файл с данными.  */
                db->idxfd = open(db->name, oflag, mode);
                strcpy(db->name + len, ".dat");
                db->datfd = open(db->name, oflag, mode);
        } else {
                /* Открыть индкусный файл и файл с данными.  */
                db->idxfd = open(db->name, oflag);
                strcpy(db->name + len, ".dat");
                db->datfd = open(db->name, oflag);
        }

        if (db->idxfd < 0 || db->datfd < 0) {
                _db_free(db);
                return(NULL);
        }

        if ((oflag & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC)) {
                /* Если создана новая база данных, ее нужно инициализировать.
                   Блокировка для записи всего файла обеспечит атомарность
                   операции получения его характеристик и инициализации.  */
                if (P_WRITEW_LOCK(db->idxfd, 0, SEEK_SET, 0) < 0)
                        p_error_dump("p_db_open: ошибка вызова функции P_WRITEW_LOCK");

                if (fstat(db->idxfd, &statbuff) < 0)
                        p_error_sys("p_db_open: ошибка вызова функции fstat");

                if (statbuff.st_size == 0) {
                        /* Создать список из (P_NHASH_DEF + 1) указателей на
                           цепочку с нулевыми значениями.  В данном случае
                           +1 - это место для указателя на список свободных
                           записей перед таблицей.  */
                        sprintf(asciiptr, "%*d", P_PTR_SZ, 0);
                        hash[0] = 0;
                        for (i = 0; i < P_NHASH_DEF + 1; i++)
                                strcat(hash, asciiptr);
                        strcat(hash, "\n");
                        i = strlen(hash);
                        if (write(db->idxfd, hash, i) != i)
                                p_error_dump("p_db_open: ошибка инициализации индексного файла");
                }
                if (P_UN_LOCK(db->idxfd, 0, SEEK_SET, 0) < 0)
                        p_error_dump("p_db_open: ошибка вызова функции P_UN_LOCK");
        }

        p_db_rewind(db);
        return(db);
}

/*
 * Размещает в памяти и инициализирует структуру DB_s и ее буферы.
 */
static DB_s *
_db_alloc(int namelen) P_NOEXCEPT
{
        DB_s    *db;

        /* Функция calloc выделяет память и забивает ее рулями.  */
        if ((db = calloc(1, sizeof(DB_s))) == NULL)
                p_error_dump("_db_alloc: ошибка размещения структуры DB_s");
        db->idxfd = db->datfd = -1; /* дескрипторы  */

        /* Выделить место для тмени.
           +5 для ".idx" или ".dat" и нулевого байта в конце.  */
        if ((db->name = malloc(namelen + 5)) == NULL)
                p_error_dump("_db_alloc: ошибка выделения памяти для имени");

        /* Выделить память для индексного буфера и буфера данных.
           +2 для символа перевода строки и нулевого символа в конце.  */
        if ((db->idxbuf = malloc(P_IDXLEN_MAX + 2)) == NULL)
                p_error_dump("_db_alloc: ошибка распределения индексного буфера");
        if ((db->datbuf = malloc(P_DATLEN_MAX + 2)) == NULL)
                p_error_dump("_db_alloc: ошибка распределения буфера данных");

        return(db);
}

/*
 * Закрывает доступ к базе данных.
 */
void
p_db_close(P_DBHANDLE_t h) P_NOEXCEPT
{
        _db_free((DB_s *)h); /* закрывает дескрипторы, освобождает память  */
}

/*
 * Освобождает память, занимаемую структурой DB_s и буферами.
 * А также закрывает дескрипторы файлов, которые могут быть открыты.
 */
static void
_db_free(DB_s *db) P_NOEXCEPT
{
        if (db->idxfd >= 0)
                close(db->idxfd);
        if (db->datfd >= 0)
                close(db->datfd);
        if (db->idxbuf != NULL)
                free(db->idxbuf);
        if (db->datbuf != NULL)
                free(db->datbuf);
        if (db->name != NULL)
                free(db->name);
        free(db);
}

/*
 * Извлекает одну запись.
 * Возвращает указатель на строку с данными.
 */
char *
p_db_fetch(P_DBHANDLE_t h, const char *key) P_NOEXCEPT
{
        DB_s    *db = h;
        char    *ptr;

        if (_db_find_and_lock(db, key, 0) < 0) {
                ptr = NULL; /* ошибка, запись не найдена  */
                db->cnt_fetcherr++;
        } else {
                ptr = _db_readdat(db); /* вернуть указатель на строку с данными  */
                db->cnt_fetchok++;
        }

        /* Снять блокировку с цепочки, установленную в _db_find_and_lock.  */
        if (P_UN_LOCK(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
                p_error_dump("p_db_fetch: ошибка вызова функции P_UN_LOCK");

        return(ptr);
}

/*
 * Ищет заданную запись.
 * Вызывается из p_db_delete, p_db_fetch и p_db_store.  Устанавливает блокировку
 * на цепочку из хеш-таблицы.
 */
static int
_db_find_and_lock(DB_s *db, const char *key, int writelock) P_NOEXCEPT
{
        off_t   offset, nextoffset;

        /* Рассчитать значение хеша для данного ключа и найти смещение
           соответствующей цепочки в хеш-таблице.  С этого места
           начинается поиск.  Прежде всего мы должны рассчитать смещение в
           хеш-таблице для данного ключа.  */
        db->chainoff = (_db_hash(db, key) * P_PTR_SZ) + db->hashoff;
        db->ptroff = db->chainoff;

        /* Здесь устанавливается блокировка.  Вызывающая функция должна
           снять ее.
           Внимание: блокировка устанавливается только на первый байт.  */
        if (writelock) {
                if (P_WRITEW_LOCK(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
                        p_error_dump("_db_find_and_lock: ошибка вызова P_WRITEW_LOCK");
        } else {
                if (P_READW_LOCK(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
                        p_error_dump("_db_find_and_lock: ошибка вызова P_READW_LOCK");
        }

        /* Получить смещение первой записи в данной цепочке от начала
           индексного файла (может быть 0).  */
        offset = _db_readptr(db, db->ptroff);
        while (offset != 0) {
                nextoffset = _db_readidx(db, offset);
                if (strcmp(db->idxbuf, key) == 0)
                        break;       /* найдено совпадение  */
                db->ptroff = offset; /* смещение данной записи  */
                offset = nextoffset; /* переход к следующей записи  */
        }

        /* offset == 0 означает ошибку (запись не найдена).  */
        return(offset == 0 ? -1 : 0);
}

/*
 * Вычисляет значение хеша по ключу.
 */
static P_DBHASH_t
_db_hash(DB_s *db, const char *key) P_NOEXCEPT
{
        P_DBHASH_t      hval = 0;
        char            c;
        int             i;

        for (i = 1; (c = *key++) != 0; i++)
                hval += c * i; /* произведение ASCII-кода символа и его индекса  */

        return(hval % db->nhash);
}

/*
 * Читает поле указателя на цепочку из индексного файла:
 * указатель на список свободных записей, на цепочку из хеш-таблицы или на
 * индексную запись в цепочке.
 */
static off_t
_db_readptr(DB_s *db, off_t offset) P_NOEXCEPT
{
        char    asciiptr[P_PTR_SZ + 1];

        if (lseek(db->idxfd, offset, SEEK_SET) == 1)
                p_error_dump("_db_readptr: ошибка перемещения на поле с указателем");
        if (read(db->idxfd, asciiptr, P_PTR_SZ) != P_PTR_SZ)
                p_error_dump("_db_readptr: ошибка чтения поля с указателем");

        asciiptr[P_PTR_SZ] = 0; /* завершающий нулевой символ  */
        return(atol(asciiptr));
}

/*
 * Читает следующую индексную запись, начиная с указанного смещения в
 * индексном файле.  Индексная запись считывается в буфер db->idxbuf,
 * а символы-разделители замещаются нулевыми байтами.  Если все в
 * порядке, в db->datoff и db->datlen записываются смещение и длина
 * соответствующей записи из файла с данными.
 */
static off_t
_db_readidx(DB_s *db, off_t offset) P_NOEXCEPT
{
        ssize_t         i;
        char            *ptr1, *ptr2;
        char            asciiptr[P_PTR_SZ + 1], asciilen[P_IDXLEN_SZ + 1];
        struct iovec    iov[2];

        /* Позиция в файле и смещение записи.  p_db_nextrec вызывает эту
           функцию с offset==0, что означает чтение из текущей позиции.
           Мы все равно должны вызывать lseek, чтобы прочитать запись.  */
        if ((db->idxoff = lseek(db->idxfd, offset,
            offset == 0 ? SEEK_CUR : SEEK_SET)) == -1)
                p_error_dump("_db_readidx: ошибка вызова функции lseek");

        /* Прочитать длину записи и указатель на следующую запись в начале
           текущей индексной записи.  Это позволит нам прочитать оставшуюся
           часть индексной записи.  */
        iov[0].iov_base = asciiptr;
        iov[0].iov_len  = P_PTR_SZ;
        iov[1].iov_base = asciilen;
        iov[1].iov_len  = P_IDXLEN_SZ;
        if ((i = readv(db->idxfd, &iov[0], 2)) != P_PTR_SZ + P_IDXLEN_SZ) {
                if (i == 0 && offset == 0)
                        return(-1); /* признак конца файла для p_db_nextrec  */
                p_error_dump("_db_readidx: ошибка readv при чтении индексной записи");
        }

        /* Это возвращаемое значение, всегда >= 0.  */
        asciiptr[P_PTR_SZ] = 0;      /* завершающий нулевой символ  */
        db->ptrval = atol(asciiptr); /* смещение следующей записи в цепочке  */

        asciilen[P_IDXLEN_SZ] = 0;   /* завершающий нулевой символ  */
        if ((db->idxlen = atol(asciilen)) < P_IDXLEN_MIN ||
            db->idxlen > P_IDXLEN_MAX)
                p_error_dump("_db_readidx: неверная нулевая записи");

        /* Теперь будет прочитана сама запись.  Мы прочитаем ее в индексный
           буфер, который был распределен при открытии базы данных.  */
        if ((i = read(db->idxfd, db->idxbuf, db->idxlen)) != db->idxlen)
                p_error_dump("_db_readidx: ошибка чтения индексной записи");
        if (db->idxbuf[db->idxlen-1] != P_NEWLINE) /* проверка целостности  */
                p_error_dump("_db_readidx: отсутствует символ перевода строки");
        db->idxbuf[db->idxlen-1] = 0; /* заменить NL нулевым символом  */

        /* Найти символы-разделители в индексной записи.  */
        if ((ptr1 = strchr(db->idxbuf, P_SEP)) == NULL)
                p_error_dump("_db_readidx: отсутствует первый разделитель");
        *ptr1++ = 0; /* заменить P_SEP нулевым символом  */

        if ((ptr2 = strchr(ptr1, P_SEP)) == NULL)
                p_error_dump("_db_readidx: отсутствует второй разделитель");
        *ptr2++ = 0; /* заменить P_SEP нулевым символом  */

        if (strchr(ptr2, P_SEP) != NULL)
                p_error_dump("_db_readidx: слишком много символов-разделителей");

        /* Получить смещение и длину записи с данными.  */
        if ((db->datoff = atol(ptr1)) < 0)
                p_error_dump("_db_readidx: смещение записи с данными < 0");
        if ((db->datlen = atol(ptr2)) < 0 || db->datlen > P_DATLEN_MAX)
                p_error_dump("_db_readidx: неверная длина записи с данными");

        return(db->ptrval); /* вернуть позицию следующей индексной записи  */
}

/*
 * Читает текущую запись с данными в буфер.
 * Возвращает указатель на буфер со строкой, завершающейся нулевым символом.
 */
static char *
_db_readdat(DB_s *db) P_NOEXCEPT
{
        if (lseek(db->datfd, db->datoff, SEEK_SET) == -1)
                p_error_dump("_db_readdat: ошибка вызова функции lseek");
        if (read(db->datfd, db->datbuf, db->datlen) != db->datlen)
                p_error_dump("_db_readdat: ошибка вызова функции read");
        if (db->datbuf[db->datlen-1] != P_NEWLINE) /* проверка целостности  */
                p_error_dump("_db_readdat: отсутствует символ перевода строки");

        db->datbuf[db->datlen-1] = 0; /* заменить NL нулевым символом  */
        return(db->datbuf); /* вернуть указатель на запись с данными  */
}

/*
 * Удаляет заданную запись.
 */
int
p_db_delete(P_DBHANDLE_t h, const char *key) P_NOEXCEPT
{
        DB_s    *db = h;
        int     rc = 0; /* предполагается, что запись будет найдена  */

        if (_db_find_and_lock(db, key, 1) == 0) {
                _db_dodelete(db);
                db->cnt_delok++;
        } else {
                rc = -1; /* не найдена  */
                db->cnt_delerr++;
        }
        if (P_UN_LOCK(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
                p_error_dump("p_db_delete: ошибка вызова функции P_UN_LOCK");

        return(rc);
}

/*
 * Удаляет текущую запись, заданную в структуре DB_s.
 * Эта функция вызывается из p_db_delete и p_db_store после того, как
 * запись будет найдена функцией _db_find_and_lock.
 */
static void
_db_dodelete(DB_s *db) P_NOEXCEPT
{
        int     i;
        char    *ptr;
        off_t   freeptr, saveptr;

        /* Очистить индексный буфер и буфер с данными, забив их пробелами.  */
        for (ptr = db->datbuf, i = 0; i < db->datlen - 1; i++)
                *ptr++ = P_SPACE;
        *ptr = 0; /* завершающий нулевой символ для _db_writedat  */
        ptr = db->idxbuf;
        while (*ptr)
                *ptr++ = P_SPACE;

        /* Мы должны заблокировать список свободных записей.  */
        if (P_WRITEW_LOCK(db->idxfd, P_FREE_OFF, SEEK_SET, 1) < 0)
                p_error_dump("_db_dodelete: ошибка вызова функции P_WRITEW_LOCK");

        /* Записать очищенную запись с данными.  */
        _db_writedat(db, db->datbuf, db->datoff, SEEK_SET);

        /* Прочитать указатель на первую запись в списке свободных записей.
           На его место будет записан указатель на удаленную запись.
           Это означает, что удаляемая запись вставляется в начало списка.  */
        freeptr = _db_readptr(db, P_FREE_OFF);

        /* Сохранить указатель на запись, следующую за удаляемой,
           прежде чем он будет затерт функцией _db_writeidx.  */
        saveptr = db->ptrval;

        /* Переписать индексную запись.  В результате чего будут переписаны
           значения длины индексной записи, позиции и длины записи с данными,
           ни одно из которых не было изменено, но так и должно быть.  */
        _db_writeidx(db, db->idxbuf, db->idxoff, SEEK_SET, freeptr);

        /* Записать новый указатель на начало списка свободных записей.  */
        _db_writeptr(db, P_FREE_OFF, db->idxoff);

        /* Изменить указатель, который указывает на удаляемую запись.
           Мы уже упоминали, что _db_find_and_lock записывает в db->ptroff
           адрес этого указателя.  Мы запишим в этот указатель адрес записи,
           которая следует за удаляемой, то есть saveptr.  */
        _db_writeptr(db, db->ptroff, saveptr);
        if (P_UN_LOCK(db->idxfd, P_FREE_OFF, SEEK_SET, 1) < 0)
                p_error_dump("_db_dodelete: ошибка вызова функции P_UN_LOCK");
}

/*
 * Сохраняет запись с данными.
 * Вызывается из _db_dodelete (чтобы сохранить запись, заполненную
 * пробелами) и из p_db_store.
 */
static void
_db_writedat(DB_s *db, const char *data, off_t offset, int whence) P_NOEXCEPT
{
        struct iovec    iov[2];
        static char     newline = P_NEWLINE;

        /* Если мы добавляем запись в конец файла, необходимо предварительно
           установить блокировку, чтобы выполнить lseek и write атомарно.
           Если перезаписывается существующая запись, блокировка не нужна.  */
        if (whence == SEEK_END) /* добавить в конец, заблокировать весь файл  */
                if (P_WRITEW_LOCK(db->datfd, 0, SEEK_SET, 0) < 0)
                        p_error_dump("_db_writedat: ошибка вызова функции P_WRITEW_LOCK");

        if ((db->datoff = lseek(db->datfd, offset, whence)) == -1)
                p_error_dump("_db_writedat: ошибка вызова функции lseek");
        db->datlen = strlen(data) + 1; /* в datlen включен символ NL  */

        iov[0].iov_base = (char *)data;
        iov[0].iov_len  = db->datlen - 1;
        iov[1].iov_base = &newline;
        iov[1].iov_len  = 1;
        if (writev(db->datfd, &iov[0], 2) != db->datlen)
                p_error_dump("_db_writedat: ошибка вывода записи с данными");

        if (whence == SEEK_END)
                if (P_UN_LOCK(db->datfd, 0, SEEK_SET, 0) < 0)
                        p_error_dump("_db_writedat: ошибка вызова функции P_UN_LOCK");
}

/*
 * Сохраняет индексную запись.
 * Перед этой функцией вызывается _db_writedat, которая устанавливает значения
 * поля datoff и datlen в структуре DB_s, необходимые для создания индексной
 * записи.
 */
static void
_db_writeidx(DB_s *db, const char *key,
             off_t offset, int whence, off_t ptrval) P_NOEXCEPT
{
        struct iovec    iov[2];
        char            asciiptrlen[P_PTR_SZ + P_IDXLEN_SZ + 1];
        int             len;

        if ((db->ptrval = ptrval) < 0 || ptrval > P_PTR_MAX)
                p_error_quit("_db_writeidx: неверный указатель: %d", ptrval);
        sprintf(db->idxbuf, "%s%c%lld%c%ld\n", key, P_SEP,
                (long long)db->datoff, P_SEP, (long)db->datlen);
        len = strlen(db->idxbuf);
        if (len < P_IDXLEN_MIN || len > P_IDXLEN_MAX)
                p_error_dump("_db_writeidx: неверная длина");
        sprintf(asciiptrlen, "%*lld%*d", P_PTR_SZ, (long long)ptrval,
                P_IDXLEN_SZ, len);

        /* Если запись добавляется в конец файла, необходимо предварительно
           установить блокировку, чтобы выполнить lseek и write атомарно.
           Если перезаписывается существующая запись, блокировка не нужна.  */
        if (whence == SEEK_END) /* добавление в конец файла  */
                if (P_WRITEW_LOCK(db->idxfd, ((db->nhash+1)*P_PTR_SZ)+1,
                    SEEK_SET, 0) < 0)
                        p_error_dump("_db_writeidx: ошибка вызова функции P_WRITEW_LOCK");

        /* Позиция в индексном файле и смещение записи.  */
        if ((db->idxoff = lseek(db->idxfd, offset, whence)) == -1)
                p_error_dump("_db_writeidx: ошибка вызова функции lseek");

        iov[0].iov_base = asciiptrlen;
        iov[0].iov_len  = P_PTR_SZ + P_IDXLEN_SZ;
        iov[1].iov_base = db->idxbuf;
        iov[1].iov_len  = len;
        if (writev(db->idxfd, &iov[0], 2) != P_PTR_SZ + P_IDXLEN_SZ + len)
                p_error_dump("_db_writeidx: ошибка вывода в файл индексной записи");

        if (whence == SEEK_END)
                if (P_UN_LOCK(db->idxfd, ((db->nhash+1)*P_PTR_SZ)+1,
                    SEEK_SET, 0) < 0)
                        p_error_dump("_db_writeidx: ошибка вызова функции P_UN_LOCK");
}

/*
 * Записывает значение указателя в индексный файл:
 * в список свободных записей, хеш-таблицу или индексную запись.
 */
static void
_db_writeptr(DB_s *db, off_t offset, off_t ptrval) P_NOEXCEPT
{
        char    asciiptr[P_PTR_SZ + 1];

        if (ptrval < 0 || ptrval > P_PTR_MAX)
                p_error_quit("_db_writeptr: неверный указатель: %d", ptrval);
        sprintf(asciiptr, "%*lld", P_PTR_SZ, (long long)ptrval);

        if (lseek(db->idxfd, offset, SEEK_SET) == -1)
                p_error_dump("_db_writeptr: ошибка перемещения на поле с указателем");
        if (write(db->idxfd, asciiptr, P_PTR_SZ) != P_PTR_SZ)
                p_error_dump("_db_writeptr: ошибка записи в поле с указателем");
}

/*
 * Сохраняет запись в базе данных.
 * Возвращает 0 в случае успеха; 1, если запись существует и установлен флаг
 * P_DB_INSERT; -1 в случае ошибки.
 */
int
p_db_store(P_DBHANDLE_t h,
           const char *key, const char *data, int flag) P_NOEXCEPT
{
        DB_s    *db = h;
        int     rc, keylen, datlen;
        off_t   ptrval;

        if (flag != P_DB_INSERT && flag != P_DB_REPLACE &&
            flag != P_DB_STORE) {
                errno = EINVAL;
                return(-1);
        }
        keylen = strlen(key);
        datlen = strlen(data) + 1; /* +1 для символа перевода строки  */
        if (datlen < P_DATLEN_MIN || datlen > P_DATLEN_MAX)
                p_error_dump("p_db_store: неверная длина записи");

        /* _db_find_and_lock вычисляет, в какую хеш-таблицу должна быть
           добавлена новая запись (db->chainoff), независимо от того,
           существует она или нет.  Следующий вызов _db_writeptr изменит
           запись в хеш-таблице, записав в нее указатель на новую запись.
           Новая запись вставляется в начало цепочки.  */
        if (_db_find_and_lock(db, key, 1) < 0) { /* запись не найдена  */
                if (flag == P_DB_REPLACE) {
                        rc = -1;
                        db->cnt_storerr++;
                        errno = ENOENT; /* ошибка, запись не найдена  */
                        goto doreturn;
                }

                /* _db_find_and_lock уже заблокировала цепочку в хеш-таблице;
                   прочитать указатель на первую индексную запись в цепочке.  */
                ptrval = _db_readptr(db, db->chainoff);

                if (_db_findfree(db, keylen, datlen) < 0) {
                        /* Не найдена пустая запись достаточного размера,
                           Добавить новые записи в конец индексного файла и
                           файла с данными.  */
                        _db_writedat(db, data, 0, SEEK_END);
                        _db_writeidx(db, key, 0, SEEK_END, ptrval);

                        /* Значение db->idxoff было установлено в _db_writeidx.
                           Новая запись добавляется в начало цепочки
                           хеш-таблицы.  */
                        _db_writeptr(db, db->chainoff, db->idxoff);
                        db->cnt_stor1++;
                } else {
                        /* Использовать повторно пустую запись.
                           _db_findfree удалит ее из списка свободных записей и
                           установит значения db->datoff и db->idxoff.  Запись
                           добавляется в начало списка.  */
                        _db_writedat(db, data, db->datoff, SEEK_SET);
                        _db_writeidx(db, key, db->idxoff, SEEK_SET, ptrval);
                        _db_writeptr(db, db->chainoff, db->idxoff);
                        db->cnt_stor2++;
                }
        } else { /* запись найдена  */
                if (flag == P_DB_INSERT) {
                        rc = 1; /* ошибка, запись уже имеется в базе данных  */
                        db->cnt_storerr++;
                        goto doreturn;
                }

                /* Производится замена существующей записи.
                   Мы знаем, что новый ключ равен существующему, но нам нужно
                   проверить равенство размеров записей с данными.  */
                if (datlen != db->datlen) {
                        _db_dodelete(db); /* удалить существующую запись  */

                        /* Перечитать указатель из хеш-таблицы (он мог
                           измениться в процессе удаления).  */
                        ptrval = _db_readptr(db, db->chainoff);

                        /* Добавить новые записи в конец файла.  */
                        _db_writedat(db, data, 0, SEEK_END);
                        _db_writeidx(db, key, 0, SEEK_END, ptrval);

                        /* Вставить указатель на запись в начало цепочки.  */
                        _db_writeptr(db, db->chainoff, db->idxoff);
                        db->cnt_stor3++;
                } else {
                        /* Размеры данных совпадают, просто заменить запись.  */
                        _db_writedat(db, data, db->datoff, SEEK_SET);
                        db->cnt_stor4++;
                }
        }
        rc = 0; /* OK  */

doreturn:       /* снять блокировку, установленную в _db_find_and_lock  */
        if (P_UN_LOCK(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
                p_error_dump("p_db_store: ошибка вызова функции P_UN_LOCK");
        return(rc);
}

/*
 * Пытается отыскать свободную индексную запись с данными нужного размера.
 * Эта функция вызывается только из p_db_store.
 */
static int
_db_findfree(DB_s *db, int keylen, int datlen) P_NOEXCEPT
{
        int     rc;
        off_t   offset, nextoffset, saveoffset;

        /* Заблокировать указатель на список свободных записей.  */
        if (P_WRITEW_LOCK(db->idxfd, P_FREE_OFF, SEEK_SET, 1) < 0)
                p_error_dump("_db_findfree: ошибка вызова функции P_WRITEW_LOCK");

        /* Прочитать указатель на первую запись в списке.  */
        saveoffset = P_FREE_OFF;
        offset = _db_readptr(db, saveoffset);

        while (offset != 0) {
                nextoffset = _db_readidx(db, offset);
                if (strlen(db->idxbuf) == keylen && db->datlen == datlen)
                        break; /* совпадение найдено  */
                saveoffset = offset;
                offset = nextoffset;
        }

        if (offset == 0) {
                rc = -1; /* совпадений не найдено  */
        } else {
                /* Найдена запись требуемого размера.
                   Индексная запись была прочитана ранее в _db_readidx, которая
                   установила значение db->ptrval.  Кроме того, saveoffset
                   указывает на запись в списке свободных записей,
                   соответствующую найденной записи.  Мы записываем в нее
                   значение db->ptrval, исключая тем самым гайденную запись из
                   списка свободных записей.  */
                _db_writeptr(db, saveoffset, db->ptrval);
                rc = 0;

                /* Обратите внимание: _db_readidx записывает значение в
                   db->idxoff и в db->datoff.  Это обстоятельство используется
                   вызывающей функцией p_db_store для вывода новых записей в
                   файл.  */
        }

        /* Снять блокировку со списка свободных записей.  */
        if (P_UN_LOCK(db->idxfd, P_FREE_OFF, SEEK_SET, 1) < 0)
                p_error_dump("_db_findfree: ошибка вызова функции P_UN_LOCK");

        return(rc);
}

/*
 * Переход к первой записи для функции p_db_nextrec.
 * Автоматически вызывается из p_db_open.
 * Должна вызываться перед первым обращением к p_db_nextrec.
 */
void
p_db_rewind(P_DBHANDLE_t h) P_NOEXCEPT
{
        DB_s    *db = h;
        off_t   offset;

        offset = (db->nhash + 1) * P_PTR_SZ; /* +1 для списка свободных
                                                записей  */

        /* Просто устанавливаем текущую позицию в файле для данного процесса на
           первую индексную запись - блокировка не требуется.  +1, чтобы
           перешагнуть символ перевода строки в конце хеш-таблицы.  */
        if ((db->idxoff = lseek(db->idxfd, offset+1, SEEK_SET)) == -1)
                p_error_dump("p_db_rewind: ошибка вызова функции lseek");
}

/*
 * Возвращает следующую запись.
 * Мы просто двигается по индексному файлу, игнорируя удаленные записи.
 * Перед первым обращением к этой функции должна быть вызвана
 * функция p_db_rewind.
 */
char *
p_db_nextrec(P_DBHANDLE_t h, char *key) P_NOEXCEPT
{
        DB_s    *db = h;
        char    c;
        char    *ptr;

        /* На список свободных записей устанавливается блокировка для чтения,
           чтобы в процессе чтения нельзя было удалить запись.  */
        if (P_READW_LOCK(db->idxfd, P_FREE_OFF, SEEK_SET, 1) < 0)
                p_error_dump("p_db_nextrec: ошибка вызова функции P_READW_LOCK");

        do {
                /* Прочитать рчередную запись.  */
                if (_db_readidx(db, 0) < 0) {
                        ptr = NULL; /* конец индексного файла  */
                        goto doreturn;
                }

                /* Проверить, не заполнен ли ключ пробелами (пустая запись).  */
                ptr = db->idxbuf;
                while ((c = *ptr++) != 0 && c == P_SPACE)
                        ; /* перейти к первому символу, отличному от пробела  */
        } while (c == 0); /* повторять, пока не встретится непустой ключ  */

        if (key != NULL)
                strcpy(key, db->idxbuf); /* вернуть ключ  */
        ptr = _db_readdat(db);           /* вернуть указатель на буфер  */
        db->cnt_nextrec++;

doreturn:
        if (P_UN_LOCK(db->idxfd, P_FREE_OFF, SEEK_SET, 1) < 0)
                p_error_dump("p_db_nextrec: ошибка вызова функции P_UN_LOCK");
        return(ptr);
}
