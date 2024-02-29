//
// Created by 19254 on 2024/2/18.
//

#ifndef HW_TOOLS_LOG_HPP
#define HW_TOOLS_LOG_HPP
#include "fmt/format.h"
#include <ctime>

namespace utils::log {

#define PrintInfo(fstr, ...)                                                                                \
  do {                                                                                                      \
    auto now = time(nullptr);                      \
    struct tm tm_info = {0};                                                                                  \
    localtime_r(&now, &tm_info);                                                                            \
    char time_str[32];                                                                                      \
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_info);                                    \
    std::string lstr = "<I> <{}> [{}:{}] ";                                                                 \
    std::cout << fmt::format(lstr + std::string(fstr), time_str, __FILE__, __LINE__, ##__VA_ARGS__) << std::endl; \
  } while (0)

#define PrintDebug(fstr, ...)                                                          \
  do {                                                                                 \
    auto now = time(nullptr);                      \
    struct tm tm_info = {0};                                                                                  \
    localtime_r(&now, &tm_info);                                                                            \
    char time_str[32];                                                                 \
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_info);               \
    std::string lstr = "<D> <{}> [{}:{}] ";                                            \
    std::cout << fmt::format(lstr + std::string(fstr), time_str, __FILE__, __LINE__, ##__VA_ARGS__) << std::endl;     \
  } while (0)

#define PrintError(fstr, ...)                                                          \
  do {                                                                                 \
    auto now = time(nullptr);                      \
    struct tm tm_info = {0};                                                                                  \
    localtime_r(&now, &tm_info);                                                                            \
    char time_str[32];                                                                 \
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_info);               \
    std::string lstr = "<E> <{}> [{}:{}] ";                                            \
    std::cerr << fmt::format(lstr + std::string(fstr), time_str, __FILE__, __LINE__, ##__VA_ARGS__) << std::endl;     \
  } while (0)

}// namespace utils::log

#endif//HW_TOOLS_LOG_HPP
