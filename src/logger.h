#pragma once

typedef enum
{
  LOGGER_COLOR_GRAY,
  LOGGER_COLOR_GREEN,
  LOGGER_COLOR_YELLOW,
  LOGGER_COLOR_RED,

  LOGGER_COLOR_COUNT
} LoggerColor;

void logger(LoggerColor color, const char* prefix, const char* msg, ...);

#define NX_TRACE(msg, ...) logger(LOGGER_COLOR_GRAY, "TRACE", msg, ##__VA_ARGS__)
#define NX_INFO(msg, ...) logger(LOGGER_COLOR_GREEN, "INFO", msg, ##__VA_ARGS__)
#define NX_WARN(msg, ...) logger(LOGGER_COLOR_YELLOW, "WARN", msg, ##__VA_ARGS__)
#define NX_ERROR(msg, ...) logger(LOGGER_COLOR_RED, "ERROR", msg, ##__VA_ARGS__)
#define NX_ASSERT(x, msg, ...)           \
{                                     \
  if(!(x))                            \
  {                                   \
    NX_ERROR(msg, ##__VA_ARGS__);        \
    __debugbreak();                   \
  }                                   \
}
