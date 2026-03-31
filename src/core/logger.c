#include "pch.h"
#include "logger.h"

static char* LoggerColorString[LOGGER_COLOR_COUNT] = {
  [LOGGER_COLOR_GRAY] = "37",
  [LOGGER_COLOR_GREEN] = "32",
  [LOGGER_COLOR_YELLOW] = "33",
  [LOGGER_COLOR_RED] = "31",
};  

void logger(LoggerColor color, const char* prefix, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  printf("\033[%sm [%s]: ", LoggerColorString[color], prefix);
  vprintf(msg, args);
  printf("\033[0m\n");

  va_end(args);
}
