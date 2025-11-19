#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "shared/validation/network.h"

int ask_for_host_or_join() {
    int option;

    printf("1. Host game\n");
    printf("2. Join game\n");
    printf("Select an option: ");

    if (scanf("%d", &option) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    return option;
}

int ask_for_ip(char *ipv4_addr, size_t size) {
    char buffer[64];

    while (1) {
        printf("Enter server IPv4 address (default %s): ", ipv4_addr);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return -1;
        }

        if (buffer[0] == '\n') {
            return 0;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (!is_valid_ipv4(buffer)) {
            printf("Invalid IPv4 address. Try again.\n");
            continue;
        }

        strncpy(ipv4_addr, buffer, size);
        ipv4_addr[size - 1] = '\0';

        return 0;
    }
}

int ask_for_port(int *port) {
    char buffer[32];

    while (1) {
        printf("Enter server port (default %d): ", *port);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return -1;
        }

        if (buffer[0] == '\n') {
            return 0;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '\0') {
            return 0;
        }

        int valid = is_valid_port(&buffer);

        if (!valid) {
            printf("Invalid port. Must be digits only.\n");
            continue;
        }

        int temp = atoi(buffer);
        if (temp < 1 || temp > 65535) {
            printf("Port must be between 1 and 65535.\n");
            continue;
        }

        *port = temp;
        return 0;
    }
}