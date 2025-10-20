#pragma once

#include <cstdint>
#include <cstring>

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_TRACE 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_ERROR
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if LOG_LEVEL > LOG_LEVEL_OFF
#define LOG_ERROR(fmt, ...)                                                                        \
    printf("E %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)                                                                        \
    do {                                                                                           \
    } while (0)
#endif

#if LOG_LEVEL > LOG_LEVEL_ERROR
#define LOG_WARN(fmt, ...)                                                                         \
    printf("W %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_WARN(fmt, ...)                                                                         \
    do {                                                                                           \
    } while (0)
#endif

#if LOG_LEVEL > LOG_LEVEL_WARN
#define LOG_INFO(fmt, ...)                                                                         \
    printf("I %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)                                                                         \
    do {                                                                                           \
    } while (0)
#endif

#if LOG_LEVEL > LOG_LEVEL_INFO
#define LOG_TRACE(fmt, ...)                                                                        \
    printf("T %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_TRACE(fmt, ...)                                                                        \
    do {                                                                                           \
    } while (0)
#endif
