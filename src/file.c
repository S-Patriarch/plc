/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            file.c
 */

#include <plc/file.h>

int 
p_file_copy(const char *f_src, const char *f_dst) P_NOEXCEPT 
{
        int src = syscall(SYS_open, f_src, O_RDONLY, 0);
        if (src == -1) 
                return (P_ERROR);

        int dst = syscall(SYS_open, f_dst, O_CREAT|O_WRONLY, 0644);
        if (dst == -1) {
                syscall(SYS_close, src);
                return (P_ERROR);
        }
        
        char    buf[4096];
        long    bytes;

        while ((bytes = syscall(SYS_read, src, buf, sizeof(buf))) > 0)
                syscall(SYS_write, dst, buf, bytes);

        syscall(SYS_close, src);
        syscall(SYS_close, dst);

        return 0;
}

int 
p_file_move(const char *f_src, const char *f_dst) P_NOEXCEPT
{
        int src = syscall(SYS_open, f_src, O_RDONLY, 0);
        if (src == -1) 
                return (P_ERROR);

        int dst = syscall(SYS_open, f_dst, O_CREAT|O_WRONLY, 0644);
        if (dst == -1) {
                syscall(SYS_close, src);
                return (P_ERROR);
        }
        
        char    buf[4096];
        long    bytes;

        while ((bytes = syscall(SYS_read, src, buf, sizeof(buf))) > 0)
                syscall(SYS_write, dst, buf, bytes);

        syscall(SYS_close, src);
        syscall(SYS_close, dst);

        if (syscall(SYS_unlink, f_src) == -1)
                return (P_ERROR);

        return 0;
}

int
p_file_setfl(int fd, int flags) P_NOEXCEPT
{
        int     val;

        if ((val = fcntl(fd, F_GETFL, 0)) < 0)
                return (P_ERROR);

        val |= flags; /* включить флаги  */

        if (fcntl(fd, F_SETFL, val) < 0)
                return (P_ERROR);

        return 0;
}

pid_t 
p_lock_test(int fd, int type, off_t offset, int whence, off_t len) P_NOEXCEPT 
{
        struct flock fl;

        fl.l_type = type;       /* F_RDLCK или F_WRLCK  */
        fl.l_start = offset;    /* смещение в байтах относительно l_whence  */
        fl.l_whence = whence;   /* SEEL_SET, SEEK_CUR, SEEK_END  */
        fl.l_len = len;         /* количество байтов (0 - до конца файла)  */

        if (fcntl(fd, F_GETLK, &fl) < 0)
                p_error_sys("E: fcntl error");

        if (fl.l_type == F_UNLCK)
                return(0);      /* ложь, заданная область не заблокирована  
                                   другим процессом  */
        return(fl.l_pid);       /* истина, вернуть pid владельца блокировки  */
}

int 
p_lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) P_NOEXCEPT 
{
        struct flock fl;

        fl.l_type = type;       /* F_RDLCK, F_WRLCK, F_UNLCK  */
        fl.l_start = offset;    /* смещение в байтах относительно l_whence  */
        fl.l_whence = whence;   /* SEEL_SET, SEEK_CUR, SEEK_END  */
        fl.l_len = len;         /* количество байтов (0 - до конца файла)  */

        return(fcntl(fd, cmd, &fl));
}

int
p_lock_file(int fd) P_NOEXCEPT 
{
        struct flock fl;

        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;

        return(fcntl(fd, F_SETLK, &fl));
}

