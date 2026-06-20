/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            sds.c
 */

#include <plc/sds.h>

/* Вспомогательная функция.  */
static inline struct sds_s *
p_sds_header(const char *s) P_NOEXCEPT 
{
        return((struct sds_s *)(s - sizeof(struct sds_s)));
}

char *
p_sds_reserve(char *s, size_t addlen) P_NOEXCEPT 
{
        struct sds_s    *hdr, *newhdr;
        size_t          len, newlen;
        size_t          free = p_sds_avail(s);

        if (free >= addlen)
                return(s);

        len = p_sds_len(s);
        hdr = p_sds_header(s);
        newlen = (len +addlen) * 2; /* стратегия удвоения  */

        newhdr = (struct sds_s *)realloc(hdr, sizeof(struct sds_s) + newlen + 1);
        if (!newhdr)
                return(NULL);

        newhdr->free = newlen - len;
        newhdr->len = len;

        return(newhdr->buf);
}

char *
p_sds_removefreespace(char *s) P_NOEXCEPT 
{
        struct sds_s *hdr = p_sds_header(s);
        struct sds_s *newhdr;

        newhdr = (struct sds_s *)realloc(hdr, sizeof(struct sds_s) + hdr->len + 1);
        if (!newhdr)
                return(NULL);

        newhdr->free = 0;

        return(newhdr->buf);
}

char *
p_sds_newlen(const void *s, size_t slen) P_NOEXCEPT 
{
        struct sds_s *hdr;

        hdr = (struct sds_s *)malloc(sizeof(struct sds_s) + slen + 1);
        if (!hdr)
                return(NULL);

        hdr->len = slen;
        hdr->free = 0;

        if (s && slen)
                memcpy(hdr->buf, s, slen);
        hdr->buf[slen] = '\0';

        return(hdr->buf);
}

char *
p_sds_new(const char *s) P_NOEXCEPT 
{
        size_t slen = (s == NULL) ? 0 : strlen(s);
        return(p_sds_newlen(s, slen));
}

char *
p_sds_empty(void) P_NOEXCEPT 
{
        return(p_sds_newlen("", 0));
}

char *
p_sds_dup(const char *s) P_NOEXCEPT 
{
        return(p_sds_newlen(s, p_sds_len(s)));
}

void
p_sds_free(char *s) P_NOEXCEPT 
{
        if (s) 
                free(p_sds_header(s));
}

char *
p_sds_catlen(char *s, const void *t, size_t len) P_NOEXCEPT 
{
        size_t curlen = p_sds_len(s);

        s = p_sds_reserve(s, len);
        if (!s)
                return(NULL);

        struct sds_s *hdr = p_sds_header(s);
        memcpy(s + curlen, t, len);
        hdr->len = curlen + len;
        hdr->free = hdr->free - len;
        s[curlen + len] = '\0';

        return(s);
}

char *
p_sds_cat(char *s, const char *t) P_NOEXCEPT 
{
        return(p_sds_catlen(s, t, strlen(t)));
}

char *
p_sds_catsds(char *s, const char *t) P_NOEXCEPT 
{
        return(p_sds_catlen(s, t, p_sds_len(t)));
}

char *
p_sds_cpylen(char *s, const void *t, size_t len) P_NOEXCEPT 
{
        struct sds_s    *hdr = p_sds_header(s);
        size_t          buflen = hdr->len + hdr->free;

        if (buflen < len) {
                s = p_sds_reserve(s, len - buflen);
                if (!s)
                        return(NULL);
                hdr = p_sds_header(s);
                buflen = hdr->len + hdr->free;
        }

        memcpy(s, t, len);
        s[len] = '\0';
        hdr->len = len;
        hdr->free = buflen - len;

        return(s);
}

char *
p_sds_cpy (char *s, const char*t) P_NOEXCEPT 
{
        return(p_sds_cpylen(s, t, strlen(t)));
}

char *
p_sds_catvprintf(char *s, const char *fmt, va_list ap) P_NOEXCEPT 
{
        va_list         cpy;
        char            *buf, *t;
        size_t          buflen = 16;

        for (;;) {
                buf = (char *)malloc(buflen);
                if (!buf)
                        return(NULL);

                buf[buflen - 2] = '\0';
                va_copy(cpy, ap);
                vsnprintf(buf, buflen, fmt, cpy);
                va_end(cpy);

                if (buf[buflen - 2] != '\0') {
                        free(buf);
                        buf = NULL;
                        buflen *= 2;
                        continue;
                }
                break;
        }

        t = p_sds_cat(s, buf);
        free(buf);
        buf = NULL;

        return(t);
}

char *
p_sds_catprintf(char *s, const char *fmt, ...) P_NOEXCEPT 
{
        va_list ap;
        va_start(ap, fmt);
        s = p_sds_catvprintf(s, fmt, ap);
        va_end(ap);

        return(s);
}

char *
p_sds_catfmt(char *s, const char *fmt, ...) P_NOEXCEPT 
{
        va_list ap;
        va_start(ap, fmt);
        s = p_sds_catvprintf(s, fmt, ap);
        va_end(ap);

        return(s);
}

void 
p_sds_trim(char *s, const char *cset) P_NOEXCEPT 
{
        struct sds_s    *hdr = p_sds_header(s);
        char            *start, *end, *sp, *ep;
        size_t          len;

        sp = start = s;
        ep = end = s + p_sds_len(s) - 1;

        while (sp <= end && strchr(cset, *sp))
                sp++;
        while (ep > sp && strchr(cset, *ep))
                ep--;

        len = (sp > ep) ? 0 : ((ep - sp) + 1);

        if (hdr->buf != sp)
                memmove(hdr->buf, sp, len);
        hdr->buf[len] = '\0';
        hdr->free = hdr->free + (hdr->len - len);
        hdr->len = len;
}

void 
p_sds_range(char *s, ptrdiff_t start, ptrdiff_t end) P_NOEXCEPT 
{
        struct sds_s    *hdr = p_sds_header(s);
        size_t          newlen, len = p_sds_len(s);

        if (len == 0)
                return;

        /* приведение отрицательных индексов  */
        if (start < 0) {
                start = len + start;
                if (start < 0)
                        start = 0;
        }
        if (end < 0) {
                end = len + end;
                if (end < 0)
                        end = 0;
        }

        newlen = (start > end) ? 0 : (end - start + 1);
        if (newlen > (len - start))
                newlen = len - start;

        if (start >= (ptrdiff_t)len) {
                newlen = 0;
                start = 0;
        }

        if (newlen && start)
                memmove(hdr->buf, hdr->buf + start, newlen);
        hdr->buf[newlen] = '\0';
        hdr->free = hdr->free + (hdr->len - newlen);
        hdr->len = newlen;
}

char **
p_sds_split(const char *s, const char *sep, int *count) P_NOEXCEPT 
{
        int             elements = 0, slots = 5, i;
        char            **tokens;
        const char      *p, *start;
        size_t          seplen = strlen(sep);

        if (seplen == 0) {
                *count = 0;
                return(NULL);
        }

        tokens = (char **)malloc(sizeof(char *) * slots);
        if (!tokens)
                return(NULL);

        start = s;
        p = s;

        for (;;) {
                /* поиск разделителя  */
                p = strstr(p, sep);

                if (p) {
                        /* если разделитель - создаем токен  */
                        if (elements >= slots) {
                                slots *= 2;
                                tokens = (char **)realloc(tokens, 
                                                         sizeof(char *) * slots);
                                if (!tokens)
                                        goto cleanup;
                        }
                        tokens[elements++] = p_sds_newlen(start, p - start);
                        p += seplen;
                        start = p;
                } else {
                        /* последний токен  */
                        if (elements >= slots) {
                                slots *= 2;
                                tokens = (char **)realloc(tokens, 
                                                         sizeof(char *) * slots);
                                if (!tokens)
                                        goto cleanup;
                        }
                        tokens[elements++] = p_sds_newlen(start, strlen(start));
                        break;
                }
        }

        *count = elements;
        return(tokens);

cleanup:
        for (i = 0; i < elements; i++)
                p_sds_free(tokens[i]);
        free(tokens);
        tokens = NULL;
        *count = 0;
        return(NULL);
}

void 
p_sds_splitfree(char **s, size_t count) P_NOEXCEPT 
{
        size_t i;

        if (s == NULL)
                return;

        /* освобождаем каждую отдельную строку  */
        for (i = 0; i < count; i++)
                p_sds_free(s[i]);

        /* освобождаем сам массив указателей  */
        free(s);
}

void 
p_sds_tolower(char *s) P_NOEXCEPT 
{
        struct sds_s    *hdr = p_sds_header(s);
        size_t          i;

        for (i = 0; i < hdr->len; i++)
                s[i] = (char)tolower((unsigned char)s[i]);
}

void 
p_sds_toupper(char *s) P_NOEXCEPT 
{
        struct sds_s    *hdr = p_sds_header(s);
        size_t          i;

        for (i = 0; i < hdr->len; i++)
                s[i] = (char)toupper((unsigned char)s[i]);
}

