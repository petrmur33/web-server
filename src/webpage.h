#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

#define TIME_LEN 100

#define HTML_ROOT "<!DOCTYPE html>" \
                  "<html>" \
                  "%s" \
                  "</html>"
#define HTML_BODY "<body>" \
                  "%s" \
                  "</body>"
#define HTML_HEAD "<head>" \
                  "%s" \
                  "</head>"
#define HTML_TITLE "<title>" \
                   "%s" \
                   "</title>"
#define HTML_H1 "<h1>" \
                "%s" \
                "</h1>"
#define HTML_H2 "<h2>" \
                "%s" \
                "</h2>"
#define HTML_A "<a%s>" \
                "%s" \
                "</a>"
                
#define HTML_META "<meta%s>"

typedef struct __buffer tag;

void add_content(tag *webpage, const char *content);
void add_tag(tag *webpage, tag *_tag, bool free_tag_memory);
void close_tag(tag *_tag);
void create_tag(char *tag_string, tag *_tag);
void delete_tag(tag *_tag);

void add_parameter(tag *_tag, char *parameter_str);
void close_parameters(tag *_tag);