#include "utils.h"

void str_reverse(char *str, size_t str_len) {
    for (size_t i = 0; i < str_len / 2; i++) {
        char tmp = str[i];
        str[i] = str[str_len - i - 1];
        str[str_len - i - 1] = tmp;
    }
}

bool str_startswith(const char *hay, const char *needle) {
    size_t hay_len = strlen(hay);
    size_t needle_len = strlen(needle);
    if (needle_len > hay_len)
        return false;
    if (strncmp(hay, needle, needle_len))
        return false;
    
    return true;
}

bool str_endswith(const char *hay, const char *needle) {
    size_t hay_len = strlen(hay);
    size_t needle_len = strlen(needle);
    if (needle_len > hay_len)
        return false;
    for (int i = needle_len - 1; i >= 0; i--)
        if (needle[i] != hay[hay_len - needle_len + i])
            return false;
    return true;
}

// return 0 if succeded otherwise -1
int add_pair_to_dict(struct __dict *dict, const char *key, const char *value) {
    size_t key_len = strlen(key);
    size_t value_len = strlen(value);

    // if key or value is bigger than free buffer memory
    if ((key_len + 1 > dict->keys_buffer.size - dict->keys_buffer_end) || (value_len + 1 > dict->values_buffer.size - dict->values_buffer_end))
	    return -1;
    if (dict->dict.size - dict->dict_buffer_end - 1 < 2)
	    return -1;

    memcpy(dict->keys_buffer.buffer + dict->keys_buffer_end, key, key_len + 1);
    memcpy(dict->values_buffer.buffer + dict->values_buffer_end, value, value_len + 1);
    
    size_t *buffer = (size_t *) dict->dict.buffer;
    buffer[dict->dict_buffer_end] = dict->keys_buffer_end;
    buffer[dict->dict_buffer_end + 1] = dict->values_buffer_end;
    dict->dict_buffer_end += 2;

    dict->keys_buffer_end += key_len + 1;
    dict->values_buffer_end += value_len + 1;

    return 0;
}

// Returns NULL if key not found or error
char *get_value_by_key(struct __dict *dict, const char *key) {
    for (size_t i = 0; i < dict->dict_buffer_end; i += 2) {
	size_t *dict_buffer = (size_t *) dict->dict.buffer;
	char *selected_key = dict->keys_buffer.buffer + dict_buffer[i];
	if (strcmp(selected_key, key) == 0)
	    return dict->values_buffer.buffer + dict_buffer[i + 1];
    }

    return NULL;
}

void init_dict(struct __dict *dict, size_t dict_len, size_t buffers_size) {
    init_buffer(&(dict->keys_buffer), buffers_size);
    init_buffer(&(dict->values_buffer), buffers_size);
    init_buffer(&(dict->dict), dict_len * sizeof(size_t));
    dict->keys_buffer_end = 0;
    dict->values_buffer_end = 0;
    dict->dict_buffer_end = 0;
}

void free_dict(struct __dict *dict) {
    free_buffer(&(dict->keys_buffer));
    free_buffer(&(dict->values_buffer));
    free_buffer(&(dict->dict));
    dict->keys_buffer_end = 0;
    dict->values_buffer_end = 0;
    dict->dict_buffer_end = 0;
}

char *strcat_s(char *dest, size_t dest_size, const char *src) {
    uint32_t dest_pointer = 0,
             src_pointer  = 0;
    while (dest[dest_pointer] != '\0') dest_pointer++;
    while (src[src_pointer] != '\0' && (dest_pointer + 1) < dest_size)
        dest[dest_pointer++] = src[src_pointer++];
    dest[dest_pointer] = '\0';
    return dest;
}

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// NOTE: VERY DANGEROUS TO USE SINCE RETURN BUFFER IS STATIC
file_buffer *read_entire_file(const char* file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file)
        return NULL;
    
    fseek(file, 0, SEEK_SET);
    long file_length = get_file_lengths(file);
    static file_buffer buf;
    buf.buffer = calloc(file_length, 1);
    buf.size = file_length;
    fread(buf.buffer, 1, file_length, file);

    fclose(file);

    return &buf;
}

file_buffer *read_entire_file_to_buffer(const char* file_path, file_buffer *buf) {
    FILE *file = fopen(file_path, "rb");
    if (!file)
        return NULL;
    
    fseek(file, 0, SEEK_SET);
    long file_length = get_file_lengths(file);
    if (file_length > buf->size)
        return NULL;
    
    fread(buf->buffer, 1, file_length, file);

    fclose(file);

    return buf;
}

// NOTE: Returns zero if file failed to open
// TODO: this function cant tell that file is failed to open because any value in size_t range may be file size
size_t get_file_lengthp(const char* file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file)
        return 0;
    
    long start_position = ftell(file);
    fseek(file, 0, SEEK_END);
    long ret = ftell(file);
    fseek(file, start_position, SEEK_SET);
    
    fclose(file);
    
    return ret;
}

size_t get_file_lengths(FILE *file) {
    long start_position = ftell(file);
    fseek(file, 0, SEEK_END);
    long ret = ftell(file);
    fseek(file, start_position, SEEK_SET);
    
    return ret;
}

char *format_time(time_t time, char *dest, size_t dest_size) {
    struct tm *tm_struct = gmtime(&time);
    strftime(dest, dest_size, "%a, %d %b %Y %H:%M:%S GMT", tm_struct);
    return dest;
}

void init_buffer(struct __buffer *buf, size_t target_size) {
    buf->buffer = (int8_t *) calloc(target_size, 1);
    if (buf->buffer == NULL) {
        perror("Memory is not allocated\n");
        return;
    }
    buf->size = target_size;
}

void clear_buffer(struct __buffer *buf, int filler) {
    if (buf->size <= 0) return;
    memset(buf->buffer, filler, buf->size);
}

void realloc_buffer(struct __buffer *buf, size_t new_size) {
    buf->buffer = (int8_t *) realloc(buf->buffer, new_size);
    if (buf->buffer == NULL) {
        perror("Memory not allocated\n");
        return;
    }
    buf->size = new_size;
}

void free_buffer(struct __buffer *buf) {
    free(buf->buffer);
    buf->buffer = NULL;
    buf->size = 0;
}