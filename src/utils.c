#include "pch.h"
#include "utils.h"
#include "logger.h"

char *read_file(const char *filePath)
{
  FILE *file;
  char *output;

  if(fopen_s(&file, filePath, "rb") == 0)
  {
    long bufferCount;
    fseek(file, 0, SEEK_END);
    bufferCount = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc((bufferCount + 1) * sizeof(char));

    fread(buffer, sizeof(char), bufferCount, file);
    buffer[bufferCount] = '\0';
    output = buffer;

    fclose(file);
  }
  else
  {
    NX_ASSERT(false, "Failed to Open the File: %s", filePath);
  }

  return output;
}
