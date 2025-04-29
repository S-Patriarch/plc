/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : void_cast.h
 */

#ifndef __PLC_VOID_CAST_H
#define __PLC_VOID_CAST_H       1

#ifdef __cplusplus
        struct void_cast {
                inline void_cast(void *x_) : x(x_) { }
                inline void_cast(void const *x_) : x((void *)x_) { }
                template <typename T> inline operator T() { return (T)x; }
                void *x;
        };
        #define VOID_CAST(x) void_cast(x)
#else
        #define VOID_CAST(x) x
#endif

#endif  /* __PLC_VOID_CAST_H */
