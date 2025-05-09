#pragma once

#include "webpage.h"

struct __buffer get_static_page(const char *file_path);
struct __buffer generate_welcome_page();