#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

// Enum con nombres de colores para los file descriptors y colores básicos
typedef enum {
    STDIN = 0,
    STDOUT,
    STDERR,
    STDMARK,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    GRAY,
    ORANGE,
    PURPLE,
    BROWN,
    PINK,
    LIME,
    NAVY,
    TEAL,
    OLIVE,
    MAROON,
    SILVER,
    GOLD,
    FDS_COUNT // Total de colores
} fds;

// Array de colores asociados a cada valor del enum
static const uint32_t fileDescriptorStyle[FDS_COUNT] = {
    0x00000000, // FDS_STDIN - negro
    0x00FFFFFF, // FDS_STDOUT - blanco
    0x00FF0000, // FDS_STDERR - rojo
    0x0000FF00, // FDS_STDMARK - verde
    0x003366FF, // FDS_BLUE - azul celeste más visible
    0x00FFFF00, // FDS_YELLOW
    0x0000FFFF, // FDS_CYAN
    0x00FF00FF, // FDS_MAGENTA
    0x00808080, // FDS_GRAY
    0x00FFA500, // FDS_ORANGE
    0x00800080, // FDS_PURPLE
    0x00A52A2A, // FDS_BROWN
    0x00FFC0CB, // FDS_PINK
    0x0000FF00, // FDS_LIME
    0x00000080, // FDS_NAVY
    0x00008080, // FDS_TEAL
    0x00808000, // FDS_OLIVE
    0x00800000, // FDS_MAROON
    0x00C0C0C0, // FDS_SILVER
    0x00FFD700  // FDS_GOLD
};

#endif // COLORS_H
