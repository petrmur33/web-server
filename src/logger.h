#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "multithreading.h"

#define LOGGER_MESSAGE_FORMAT "[%s] [%lu] %s %s\n"
#define LOGGER_TIME_FORMAT "%d.%m.%Y %H:%M:%S"
#define LOGGER_TIME_LEN 100

typedef enum {
    LOGGER_DEBUG,
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_CRITICAL
} logger_modes;

typedef struct {
    FILE *stream;
    logger_modes mode;
} logger;

logger global_logger;

int logger_create_global(const char *file_path, bool new_file, logger_modes mode);
logger logger_create(const char *file_path, bool new_file, logger_modes mode);
void logger_write(logger *_logger, const char *message, logger_modes mode);
void logger_write_global(const char* message, logger_modes mode);
int logger_close(logger *_logger);