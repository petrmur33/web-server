#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define ERROR_MESSAGE_LEN 100

struct __buffer {
    int8_t *buffer;
    size_t size;
};

typedef struct __buffer file_buffer;
typedef struct __buffer tmp_buffer;

struct __dict {
    struct __buffer keys_buffer;
    struct __buffer values_buffer;
    struct __buffer dict;
    size_t keys_buffer_end;
    size_t values_buffer_end;
    size_t dict_buffer_end;
};

void init_buffer(struct __buffer *buf, size_t target_size);
void realloc_buffer(struct __buffer *buf, size_t new_size);
void free_buffer(struct __buffer *buf);
void clear_buffer(struct __buffer *buf, int filler);

void init_dict(struct __dict *dict, size_t dict_size, size_t buffers_size);
void free_dict(struct __dict *dict);
int add_pair_to_dict(struct __dict *dict, const char *key, const char *value); // return 0 if succeded otherwise -1
char *get_value_by_key(struct __dict *dict, const char *key);

file_buffer *read_entire_file(const char* file_path);
file_buffer *read_entire_file_to_buffer(const char* file_path, file_buffer *buf);
size_t get_file_lengthp(const char* file_path);
size_t get_file_lengths(FILE *file);

char *strcat_s(char *dest, size_t dest_size, const char *src);
bool str_endswith(const char *hay, const char *needle);
bool str_startswith(const char *hay, const char *needle);
void str_reverse(char *str, size_t str_len);

void error(const char *msg);

char *format_time(time_t time, char *dest, size_t dest_size);
