#ifndef CLIENT_GAME_MENU_DIALOGS_H
#define CLIENT_GAME_MENU_DIALOGS_H

#include <stdio.h>

int ask_for_host_or_join();
int ask_for_ip(char *ipv4_addr, size_t size);
int ask_for_port(int *port);

#endif