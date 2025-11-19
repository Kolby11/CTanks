#include <stdio.h>

int is_valid_ipv4(const char *ip) {
    int dots = 0;
    int num = 0;

    if (!ip || *ip == '\0')
        return 0;

    const char *p = ip;
    while (*p) {
        if (*p == '.') {
            dots++;
            if (dots > 3) return 0;
            num = 0;
        } else if (isdigit((unsigned char)*p)) {
            num++;
            if (num > 3) return 0;
        } else {
            return 0;
        }
        p++;
    }

    return dots == 3;
}

int is_valid_port(const char *buffer) {
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buffer[i])) {
            return 0;
        }
    }
    return 1;
}