#include "logger.h"

static char *logger_format_time(time_t time, char *dest, size_t dest_size);

static char *logger_format_time(time_t time, char *dest, size_t dest_size) {
    struct tm *tm_struct = gmtime(&time);
    strftime(dest, dest_size, LOGGER_TIME_FORMAT, tm_struct);
    return dest;
}

// Returns 1 if logger init failed
int logger_create_global(const char *file_path, bool new_file, logger_modes mode) {
    FILE *stream = fopen(file_path, new_file ? "wb" : "ab");
    
    if (stream == NULL)
        return 1;
    
    global_logger.stream = stream;
    global_logger.mode = mode;
    
    return 0;
}

logger logger_create(const char *file_path, bool new_file, logger_modes mode) {
    logger ret;
    ret.stream = fopen(file_path, new_file ? "wb" : "ab");
    ret.mode = mode;
    return ret;
}

void logger_write_global(const char* message, logger_modes mode) {
    logger_write(&global_logger, message, mode);
}

void logger_write(logger *_logger, const char *message, logger_modes mode) {
    if (mode < _logger->mode)
        return;
    
    thread current_thread_id = thread_get_current_thread_id();
    char time_str[LOGGER_TIME_LEN];
    logger_format_time(time(NULL), time_str, LOGGER_TIME_LEN);
    
    char *logger_mode_str;
    switch (mode) {
        case LOGGER_DEBUG:
            logger_mode_str = "DEBUG";
            break;
        case LOGGER_INFO:
            logger_mode_str = "INFO";
            break;
        case LOGGER_WARNING:
            logger_mode_str = "WARNING";
            break;
        case LOGGER_ERROR:
            logger_mode_str = "ERROR";
            break;
        case LOGGER_CRITICAL:
            logger_mode_str = "CRITICAL";
            break;
    }
    fprintf(_logger->stream, LOGGER_MESSAGE_FORMAT, time_str, current_thread_id, logger_mode_str, message);
    fflush(_logger->stream);
}

// Returns 0 on success
int logger_close(logger *_logger) {
    int result = fclose(_logger->stream);
    if (result != 0)
        return 1;
    return 0;
}
