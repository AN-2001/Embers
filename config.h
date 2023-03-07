/******************************************************************************\
*  config.h                                                                    *
*                                                                              * 
*  Stores basic config data.                                                   *
*                                                                              * 
*              Written by Abed Naran                          November 2022    *
*                                                                              *
\******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H
#include "time.h"

#define EMBERS_REAL float
#define EMBERS_BUFFER_SIZE (2048)
#define EMBERS_BUFFER_SIZE_BIG (4096)

#ifdef BOOL
#undef BOOL
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define EMBERS_BOOL int
#define EMBERS_TRUE (1)
#define EMBERS_FALSE (0)

/* Ticks Per Second.                                                          */
#define EMBERS_TPS (120)

/* Small epsilon to account for floating point error.                         */
#define EMBERS_EPSILON (1e-4)

#define EMBERS_WIDTH (800)
#define EMBERS_HEIGHT (800)

#define EMBERS_SPLASH_MSG "Embers by Abed Na'ran, Compiled on " __DATE__

#define EMBERS_LOG_INFO(m)                          \
    do {                                            \
        time_t tm = time(NULL);                     \
        fprintf(stdout,                             \
                "<%d:%d:%d> [INFO] %s\n",           \
                localtime(&tm) -> tm_hour,          \
                localtime(&tm) -> tm_min,           \
                localtime(&tm) -> tm_sec,           \
                m);                                 \
    } while (0)

#define EMBERS_LOG_ERROR(m)                         \
    do {                                            \
        time_t tm = time(NULL);                     \
        fprintf(stderr,                             \
                "<%d:%d:%d> [ERROR] %s\n",          \
                localtime(&tm) -> tm_hour,          \
                localtime(&tm) -> tm_min,           \
                localtime(&tm) -> tm_sec,           \
                m);                                 \
    } while (0)

#endif /* CONFIG_H */
