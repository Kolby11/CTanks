#include <stdio.h>

void serialize_movement(int player, int x, int y, int z, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "{'player': '%d', 'move': '%d %d %d'}", player, x, y, z);
}

int deserialize_movement(const char *buffer, int *player, int *x, int *y, int *z) {
    return sscanf(buffer, "{'player': '%d', 'move': '%d %d %d'}", player, x, y, z);
}