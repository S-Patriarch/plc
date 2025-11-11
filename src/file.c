/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : file.c
 */

#include <plc/file.h>
#include <plc/plcdef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>

int 
p_file_copy(const char *f_src, const char *f_dst) 
{
        int src = syscall(SYS_open, f_src, O_RDONLY, 0);
        if (src == -1) 
                return P_ERROR;

        int dst = syscall(SYS_open, f_dst, O_CREAT|O_WRONLY, 0644);
        if (dst == -1) {
                syscall(SYS_close, src);
                return P_ERROR;
        }
        
        char    buf[4096];
        long    bytes;

        while ((bytes = syscall(SYS_read, src, buf, sizeof(buf))) > 0)
                syscall(SYS_write, dst, buf, bytes);

        syscall(SYS_close, src);
        syscall(SYS_close, dst);

        return P_SUCCESS;
}

int 
p_file_move(const char *f_src, const char *f_dst) 
{
        int src = syscall(SYS_open, f_src, O_RDONLY, 0);
        if (src == -1) 
                return P_ERROR;

        int dst = syscall(SYS_open, f_dst, O_CREAT|O_WRONLY, 0644);
        if (dst == -1) {
                syscall(SYS_close, src);
                return P_ERROR;
        }
        
        char    buf[4096];
        long    bytes;

        while ((bytes = syscall(SYS_read, src, buf, sizeof(buf))) > 0)
                syscall(SYS_write, dst, buf, bytes);

        syscall(SYS_close, src);
        syscall(SYS_close, dst);

        if (syscall(SYS_unlink, f_src) == -1)
                return P_ERROR;

        return P_SUCCESS;
}

int
p_file_setfl(int fd, int flags) 
{
        int     val;

        if ((val = fcntl(fd, F_GETFL, 0)) < 0)
                return(-1);

        val |= flags; /* включить флаги */

        if (fcntl(fd, F_SETFL, val) < 0)
                return(-1);

        return(0);
}

