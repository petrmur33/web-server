#include "webpage.h"

void delete_tag(tag *_tag) {
    free_buffer(_tag);
}

void create_tag(char *tag_string, tag *_tag) {
    size_t string_size = strlen(tag_string) + 1; // With 0 at the end
    size_t memory_size = sizeof(char) * string_size;
    init_buffer(_tag, memory_size);
    
    strcpy(_tag->buffer, tag_string);
}

void add_tag(tag *webpage, tag *_tag, bool free_tag_memory) {
    add_content(webpage, _tag->buffer);
    if (free_tag_memory)
        delete_tag(_tag);
}

void add_content(tag *webpage, const char *content) {
    size_t webpage_len = strlen(webpage->buffer);
    size_t content_len = strlen(content);
    size_t ret_len = webpage_len + content_len;

    if ((ret_len + 1) > webpage->size) {
        size_t new_memory_size = sizeof(char) * (ret_len + 1);
        realloc_buffer(webpage, new_memory_size);
    }

    char ret[ret_len + 1];
    bool prev_is_percent = false;
    bool content_placed = false;
    int ret_pointer = 0;
    for (int webpage_index = 0; webpage_index < webpage_len; webpage_index++) {
        char c = webpage->buffer[webpage_index];
        if (c == '%')
            prev_is_percent = true;
        else if (c == 's' && prev_is_percent && !content_placed) {
            prev_is_percent = false;
            for (int content_index = 0; content_index < content_len; content_index++)
                ret[ret_pointer++] = content[content_index];
            ret[ret_pointer++] = '%';
            ret[ret_pointer++] = 's';
            content_placed = true;
        }
        else {
            if (prev_is_percent) {
                ret[ret_pointer++] = '%';
                prev_is_percent = false;
            }
            ret[ret_pointer++] = c;
        }
    }

    for (int i = 0; i < ret_pointer; i++)
        webpage->buffer[i] = ret[i];
    webpage->buffer[ret_pointer] = '\0';

    // TODO: This is bad
    if (!content_placed) {
        perror("add_content content_placed is false");
        return;
    }
}

void close_tag(tag *_tag) {
    size_t tag_len = strlen(_tag->buffer);
    bool prev_is_percent = false;
    bool specifier_removed = false;
    int ret_pointer = 0;
    for (int tag_index = 0; tag_index < tag_len; tag_index++) {
        char c = _tag->buffer[tag_index];
        if (c == '%')
            prev_is_percent = true;
        else if (c == 's' && prev_is_percent && !specifier_removed) {
            prev_is_percent = false;
            specifier_removed = true;
        }
        else {
            if (prev_is_percent) {
                _tag->buffer[ret_pointer++] = '%';
                prev_is_percent = false;
            }
            _tag->buffer[ret_pointer++] = c;
        }
    }
    
    _tag->buffer[ret_pointer] = '\0';

    // TODO: This is bad
    if (!specifier_removed) {
        perror("close_tag specifier_removed is false\n");
        return;
    }
    else
        realloc_buffer(_tag, _tag->size - 2);
}

void add_parameter(tag *_tag, char *parameter_str) {
    add_content(_tag, parameter_str);
}

void close_parameters(tag *_tag) {
    close_tag(_tag);
}