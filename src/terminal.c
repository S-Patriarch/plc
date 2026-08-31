/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            terminal.c
 */

#include <plc/terminal.h>

static struct termios   save_termios;
static int              ttysavefd = -1;
static enum {
        RESET,
        RAW,
        CBREAK
} ttystate = RESET;

int
p_tty_cbreak(int fd) P_NOEXCEPT
{
        int             err;
        struct termios  buf;

        if (ttystate != RESET) {
                errno = EINVAL;
                return(-1);
        }
        if (tcgetattr(fd, &buf) < 0)
                return(-1);
        save_termios = buf; /* копия структуры  */

        /* Отключить эхо-вывод и выйти из канонического режима.  */
        buf.c_lflag &= ~(ECHO | ICANON);

        /* Случай Б: минимум 1 байт, время ожидания не ограничено.
         *           MIN > 0, TIME == 0  */
        buf.c_cc[VMIN] = 1;
        buf.c_cc[VTIME] = 0;
        if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
                return(-1);

        /* Убедиться, что были произведены все изменения.  Функция tcsetattr
         *           может вернуть 0, даже если выполнена лишь часть изменений.  */
        if (tcgetattr(fd, &buf) < 0) {
                err = errno;
                tcsetattr(fd, TCSAFLUSH, &save_termios);
                errno = err;
                return(-1);
        }
        if ((buf.c_lflag & (ECHO | ICANON)) || buf.c_cc[VMIN] != 1 ||
                buf.c_cc[VTIME] != 0) {
                /* Были произведены лишь некоторые изменения.
                 *                   Восстановить начальные настройки.  */
                tcsetattr(fd, TCSAFLUSH, &save_termios);
        errno = EINVAL;
        return(-1);
                }

                ttystate = CBREAK;
                ttysavefd = EINVAL;
                return(0);
}

int
p_tty_raw(int fd) P_NOEXCEPT
{
        int             err;
        struct termios  buf;

        if (ttystate != RESET) {
                errno = EINVAL;
                return(-1);
        }
        if (tcgetattr(fd, &buf) < 0)
                return(-1);
        save_termios = buf; /* копия структуры  */

        /* Отключить эхо-вывод и выйти из канонического режима, отключить
         *           расширенную обработку ввода, обработку символов, генерирующих
         *           сигналы.  */
        buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

        /* Не выдавать сигнал SIGINT по псевдосимволу BREAK, отключить
         *           преобразование CR->NL, отключить проверку паритета ввода, не
         *           сбрасывать 8-й бит, отключить управление выводом.  */
        buf.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

        /* Сбросить маску управления размером, отключить контроль четности.  */
        buf.c_cflag &= ~(CSIZE | PARENB);

        /* Установить размер символа 8 бит/символ.  */
        buf.c_cflag |= CS8;

        /* Отключить обработку вывода.  */
        buf.c_oflag &= ~(OPOST);

        /* Случай Б: минимум 1 байт, время ожидания не ограничено.
         *           MIN > 0, TIME == 0  */
        buf.c_cc[VMIN] = 1;
        buf.c_cc[VTIME] = 0;
        if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
                return(-1);

        /* Убедиться, что были произведены все изменения.  Функция tcsetattr
         *           может вернуть 0, даже если выполнена лишь часть изменений.  */
        if (tcgetattr(fd, &buf) < 0) {
                err = errno;
                tcsetattr(fd, TCSAFLUSH, &save_termios);
                errno = err;
                return(-1);
        }

        if ((buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
                (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
                (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
                (buf.c_oflag & OPOST) || buf.c_cc[VMIN] != 1 ||
                buf.c_cc[VTIME] != 0) {
                /* Были произведены лишь некоторые изменения.
                 *                   Восстановить начальные настройки.  */
                tcsetattr(fd, TCSAFLUSH, &save_termios);
        errno = EINVAL;
        return(-1);
                }

                ttystate = RAW;
                ttysavefd = fd;
                return(0);
}

int
p_tty_reset(int fd) P_NOEXCEPT
{
        if (ttystate == RESET)
                return(0);
        if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
                return(-1);
        ttystate = RESET;

        return(0);
}

void
p_tty_atexit(void) P_NOEXCEPT
{
        if (ttysavefd >= 0)
                p_tty_reset(ttysavefd);
}

struct termios *
p_tty_termios(void) P_NOEXCEPT
{
        return(&save_termios);
}

int
p_ptym_open(char *pts_name, int pts_namesz) P_NOEXCEPT
{
        char    *ptr;
        int     fdm, err;

        if ((fdm = posix_openpt(O_RDWR)) < 0)
                return(-1);

        if (grantpt(fdm) < 0)  /* разрешить доступ к ведомому  */
                goto errout;
        if (unlockpt(fdm) < 0) /* сбросить флаг блокировки ведомого  */
                goto errout;
        if ((ptr = ptsname(fdm)) == NULL) /* получить имя ведомого  */
                goto errout;

        /* Вернуть имя ведомого устройства.
           Завершить нулевым символом, чтобы обработать ситуацию, когда
           strlen(ptr) > pts_namesz.  */
        strncpy(pts_name, ptr, pts_namesz);
        pts_name[pts_namesz - 1] = '\0';
        return(fdm); /* вернуть дескриптор ведущего  */

errout:
        err = errno;
        close(fdm);
        errno = err;
        return(-1);
}

int
p_ptys_open(char *pts_name) P_NOEXCEPT
{
        int     fds;
#if defined(SOLARIS)
        int     err, setup;
#endif

        if ((fds = open(pts_name, O_RDWR)) < 0)
                return(-1);

#if defined(SOLARIS)
        /* Проверить: возможно, поток уже настроен должным образом
           благодаря автоматической вставке модулей.  */
        if ((setup = ioctl(fds, I_FIND, "ldterm")) < 0)
                goto errout;

        if (setup == 0) {
                if (ioctl(fds, I_PUSH, "ptem") < 0)
                        goto errout;
                if (ioctl(fds, I_PUSH, "ldterm") < 0)
                        goto errout;
                if (ioctl(fds, I_PUSH, "ttcompat") < 0) {

errout:
                        err = errno;
                        close(fds);
                        errno = err;
                        return(-1);
                }
        }
#endif
        return(fds);
}

pid_t
p_pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
           const struct termios *slave_termios,
           const struct winsize *slave_winsize) P_NOEXCEPT
{
        int     fdm, fds;
        pid_t   pid;
        char    pts_name[20];

        if ((fdm = p_ptym_open(pts_name, sizeof(pts_name))) < 0)
                return(-1);

        if (slave_name != NULL) {
                /* Вернуть имя ведомого устройства.
                   Завершить нулевым символом, чтобы обработать ситуацию, когда
                   strlen(ptr) > pts_namesz.  */
                strncpy(slave_name, pts_name, slave_namesz);
                slave_name[slave_namesz - 1] = '\0';
        }

        if ((pid = fork()) < 0) {
                return(-1);
        } else if (pid == 0) { /* дочерний процесс  */
                if (setsid() < 0)
                        return(-1);

                /* Systen V автоматически назначает управляющий терминал при
                   открытии.  */
                if ((fds = p_ptys_open(pts_name)) < 0)
                        return(-1);
                close(fdm); /* работа с ведущим pty в дочернем процессе
                               завершена  */
#if defined(BSD)
                /* Команда TIOCSCTTY - способ назначения управляющего
                   терминала в BCD.  */
                if (ioctl(fds, TIOCSCTTY, (char *)0) < 0)
                        return(-1);
#endif
                /* Инициализировать структуры termios и winsize ведомого
                   pty.  */
                if (slave_termios != NULL) {
                        if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
                                return(-1);
                }
                if (slave_winsize != NULL) {
                        if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
                                return(-1);
                }

                /* Связать stdin/stdout/stderr с терминалом в дочернем
                   процессе.  */
                if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
                        return(-1);
                if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
                        return(-1);
                if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
                        return(-1);
                if (fds != STDIN_FILENO && fds != STDOUT_FILENO &&
                    fds != STDERR_FILENO)
                        close(fds);
                return(0); /* вернуть 0 дочернему процессу, как это делает
                              fork()  */
        } else {
                /* родительский процесс  */
                *ptrfdm = fdm; /* вернуть fd ведущего pty  */
                return(pid);   /* вернуть pid дочернего процесса родителю  */
        }
}
