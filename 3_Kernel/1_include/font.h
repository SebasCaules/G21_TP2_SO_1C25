#include <stdint.h>

#ifndef FONT_H
#define FONT_H

#define HEIGHT_BITMAP 16
#define NCHARS 95

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

/**
 * @brief Draws a character on the screen at a specified position with given foreground and background colors.
 * @param c The character to be drawn.
 * @param x The x-coordinate where the character will be drawn.
 * @param y The y-coordinate where the character will be drawn.
 * @param fgcolor The color of the character (foreground color).
 * @param bgcolor The color of the character's background.
 * @param scale The scale factor for the size of the character.
 */
void drawChar(unsigned char c, int x, int y, int fgcolor, int bgcolor, int scale);

#endif
