#include "webpages.h"

// NOTE: you must free returned memory
struct __buffer get_static_page(const char *file_path) {
    size_t page_size = sizeof(char) * get_file_lengthp(file_path);
    file_buffer buf;
    init_buffer(&buf, page_size);
    read_entire_file_to_buffer(file_path, &buf);
    
    return buf;
}

// NOTE: you must free returned memory
struct __buffer generate_welcome_page() {
    tag webpage;
    create_tag(HTML_ROOT, &webpage);

    tag title;
    create_tag(HTML_TITLE, &title);
    add_content(&title, "Hello World");
    close_tag(&title);
    
    tag charset;
    create_tag(HTML_META, &charset);
    add_parameter(&charset, " charset=\"UTF-8\"");
    close_parameters(&charset);

    tag head;
    create_tag(HTML_HEAD, &head);
    add_tag(&head, &charset, true);
    add_tag(&head, &title, true);
    close_tag(&head);
    add_tag(&webpage, &head, true);

    tag h1;
    create_tag(HTML_H1, &h1);
    add_content(&h1, "petrmur-webserver");
    close_tag(&h1);

    char current_time[TIME_LEN];
    format_time(time(NULL), current_time, TIME_LEN);
    char string[TIME_LEN * 2] = "Current GM Time: ";
    strcat_s(string, TIME_LEN * 2, current_time);
    
    tag a;
    create_tag(HTML_A, &a);
    add_parameter(&a, " href=\"test.html\" ");
    close_parameters(&a);
    add_content(&a, "Testpage link");
    close_tag(&a);
    
    tag h2;
    create_tag(HTML_H2, &h2);
    add_content(&h2, string);
    close_tag(&h2);

    tag body;
    create_tag(HTML_BODY, &body);
    add_tag(&body, &h1, true);
    add_tag(&body, &h2, true);
    add_tag(&body, &a, true);
    close_tag(&body);
    add_tag(&webpage, &body, true);

    close_tag(&webpage);

    webpage.size--; // webpage have null terminator, this is bad because we shouldn't send this character to client. So this hack exclude null terminator from response
    
    return webpage;
}
