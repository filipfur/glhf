#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifndef DISABLE_LOG_STREAM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>

template <typename T> inline static std::ostream &print_glm_vec(std::ostream &os, const T &t) {
    os << '[';
    std::string delim = "";
    for (size_t i{0}; i < sizeof(T) / sizeof(float); ++i) {
        os << delim << std::fixed << std::setprecision(1) << t[i];
        delim = ", ";
    }
    os << ']';
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const glm::ivec2 &v) {
    return print_glm_vec(os, v);
}
inline std::ostream &operator<<(std::ostream &os, const glm::ivec3 &v) {
    return print_glm_vec(os, v);
}
inline std::ostream &operator<<(std::ostream &os, const glm::ivec4 &v) {
    return print_glm_vec(os, v);
}

inline std::ostream &operator<<(std::ostream &os, const glm::vec2 &v) {
    return print_glm_vec(os, v);
}
inline std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
    return print_glm_vec(os, v);
}
inline std::ostream &operator<<(std::ostream &os, const glm::vec4 &v) {
    return print_glm_vec(os, v);
}

inline std::ostream &operator<<(std::ostream &os, const glm::mat2 &m) {
    os << glm::to_string(m);
    return os;
}
inline std::ostream &operator<<(std::ostream &os, const glm::mat3 &m) {
    os << glm::to_string(m);
    return os;
}
inline std::ostream &operator<<(std::ostream &os, const glm::mat4 &m) {
    os << glm::to_string(m);
    return os;
}
#endif

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_TRACE 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...)                                                                        \
    printf("E %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(fmt, ...)                                                                         \
    printf("W %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_WARN(fmt, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...)                                                                         \
    printf("I %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOG_TRACE(fmt, ...)                                                                        \
    printf("T %s:%d " fmt "\n", __FILENAME__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_TRACE(fmt, ...)
#endif

#ifndef DISABLE_LOG_STREAM
#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOGS_ERROR(stream)                                                                         \
    std::cout << "E " << __FILENAME__ << ':' << __LINE__ << ' ' << stream << std::endl;
#else
#define LOGS_ERROR(stream)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOGS_WARN(stream)                                                                          \
    std::cout << "W " << __FILENAME__ << ':' << __LINE__ << ' ' << stream << std::endl;
#else
#define LOGS_WARN(stream)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOGS_INFO(stream)                                                                          \
    std::cout << "I " << __FILENAME__ << ':' << __LINE__ << ' ' << stream << std::endl;
#else
#define LOGS_INFO(stream)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOGS_TRACE(stream)                                                                         \
    std::cout << "T " << __FILENAME__ << ':' << __LINE__ << ' ' << stream << std::endl;
#else
#define LOGS_TRACE(stream)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOGV_ERROR(var)                                                                            \
    std::cout << "E " << __FILENAME__ << ':' << __LINE__ << ' ' << #var << ": " << var << std::endl;
#else
#define LOGV_ERROR(var)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOGV_WARN(var)                                                                             \
    std::cout << "W " << __FILENAME__ << ':' << __LINE__ << ' ' << #var << ": " << var << std::endl;
#else
#define LOGV_WARN(var)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOGV_INFO(var)                                                                             \
    std::cout << "I " << __FILENAME__ << ':' << __LINE__ << ' ' << #var << ": " << var << std::endl;
#else
#define LOGV_INFO(var)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOGV_TRACE(var)                                                                            \
    std::cout << "T " << __FILENAME__ << ':' << __LINE__ << ' ' << #var << ": " << var << std::endl;
#else
#define LOGV_TRACE(var)
#endif
#endif