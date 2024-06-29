//
// Created by 19254 on 2024/2/18.
//

#ifndef HW_TOOLS_LOG_HPP
#define HW_TOOLS_LOG_HPP
#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include <ctime>
#include <iostream>

namespace utils::log {

#ifdef _D__WIN32__
#define LOCALTIME(time_str)                                                          \
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); \
  std::tm tm_info = {0};                                                             \
  localtime_s(&tm_info, &now);                                                       \
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_info);


#else
#define LOCALTIME(time_str)    \
  auto now = time(nullptr);    \
  struct tm tm_info = {0};     \
  localtime_r(&now, &tm_info); \
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_info);

#endif

#define PrintInfo(fstr, ...)                                                           \
  do {                                                                                 \
    char time_str[32];                                                                 \
    LOCALTIME(time_str);                                                               \
    std::string lstr = "<{}> [{}:{}] ";                                            \
    std::string_view fPath = __FILE__;                                                 \
    fPath = fPath.substr(strlen(PRIOJECT_PATH), fPath.size() - strlen(PRIOJECT_PATH)); \
    spdlog::info(lstr + std::string(fstr), time_str, fPath, __LINE__, ##__VA_ARGS__);  \
  } while (0)

#define PrintDebug(fstr, ...)                                                          \
  do {                                                                                 \
    char time_str[32];                                                                 \
    LOCALTIME(time_str);                                                               \
    std::string lstr = "<{}> [{}:{}] ";                                            \
    std::string_view fPath = __FILE__;                                                 \
    fPath = fPath.substr(strlen(PRIOJECT_PATH), fPath.size() - strlen(PRIOJECT_PATH)); \
    spdlog::debug(lstr + std::string(fstr), time_str, fPath, __LINE__, ##__VA_ARGS__); \
  } while (0)

#define PrintError(fstr, ...)                                                          \
  do {                                                                                 \
    char time_str[32];                                                                 \
    LOCALTIME(time_str);                                                               \
    std::string lstr = "<{}> [{}:{}] ";                                            \
    std::string_view fPath = __FILE__;                                                 \
    fPath = fPath.substr(strlen(PRIOJECT_PATH), fPath.size() - strlen(PRIOJECT_PATH)); \
    spdlog::error(lstr + std::string(fstr), time_str, fPath, __LINE__, ##__VA_ARGS__); \
  } while (0)


}// namespace utils::log

#endif//HW_TOOLS_LOG_HPP
