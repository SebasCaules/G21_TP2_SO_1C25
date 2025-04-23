#include <videoDriver.h>

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

uint8_t fontScale = 1; // NUMERO ENTERO
uint64_t currentX = BORDER_PADDING;
uint64_t currentY = BORDER_PADDING;
uint64_t currentLinePosition = 0;

void updateTextCursor(CursorMovementType movementType);

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *)(uint64_t) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

uint64_t printChar(char c, int fgcolor, int bgcolor) {
    switch (c) {
    case '\n':
        printNewLine();
        return 1;
    case '\b':
        deleteChar();
        return -1;
    case '\t':
        return 0;
    default:
        break;
    }

    // Si es un caracter ASCII no imprimible devuelve 0
    if (c <= 31){
        return 0;
    }

    // Chequea que la palabra no se pase de los margenes de la pantalla
    if (currentX + CHAR_WIDTH * fontScale + HORIZONTAL_PADDING > WINDOW_WIDTH - BORDER_PADDING) {
        // Si no hay espacio para la plabra salta de linea
        updateTextCursor(CURSOR_NEWLINE);
        currentX = BORDER_PADDING;
        currentY += (CHAR_HEIGHT + VERTICAL_PADDING) * fontScale;
    }

    if (currentY + CHAR_HEIGHT > WINDOW_HEIGHT - BORDER_PADDING) {
        clear();
    }
    currentLinePosition++;
    drawChar(c, currentX + HORIZONTAL_PADDING, currentY, fgcolor, bgcolor, fontScale);
    currentX += (CHAR_WIDTH * fontScale + HORIZONTAL_PADDING);
    updateTextCursor(CURSOR_TYPING);
    return 1;
}

void printCharBW(char c){
    printChar(c, WHITE, BLACK);
}

void printStr(char* str, int fgcolor, int bgcolor) {
    while(*str){
        printChar(*str++, fgcolor, bgcolor);
    }
}

uint64_t printStrByLength(char* str, int fgcolor, int bgcolor, int length){
    uint64_t i, printed = 0;
	for (i = 0; i < length && str[i] != 0; i++)
		printed += printChar(str[i], fgcolor, bgcolor);
	return printed;
}

void printStrBW(char* str) {
	printStr(str, WHITE, BLACK);
}

void printNewLine() {
    updateTextCursor(CURSOR_NEWLINE);
	currentX = BORDER_PADDING;
	currentY += (CHAR_HEIGHT + VERTICAL_PADDING) * fontScale;
}

uint8_t canDelete() {
    return currentLinePosition > 0;
}

void deleteChar() {
    if (!canDelete()) {
        return;
    }
    if (currentX <= BORDER_PADDING) {
        updateTextCursor(CURSOR_PREVIOUS);
        currentY -= (CHAR_HEIGHT + VERTICAL_PADDING) * fontScale;
        currentX = WINDOW_WIDTH - BORDER_PADDING - (CHAR_WIDTH + HORIZONTAL_PADDING) * fontScale;
    } else {
        currentX -= CHAR_WIDTH * fontScale + HORIZONTAL_PADDING;
    }
    drawRectangle(currentX, currentY, CHAR_WIDTH * fontScale + HORIZONTAL_PADDING, CHAR_HEIGHT * fontScale, 0x00000000); // ACA
    updateTextCursor(CURSOR_DELETING);
    currentLinePosition--;
}

void clear() {
    drawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0x00000000);
    currentY = BORDER_PADDING;
}

void resetCursor() {
    currentY = BORDER_PADDING;
}

void drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color) {
    for (uint64_t i = 0; i <= width; i++) {
        for (uint64_t j = 0; j <= height; j++) {
            putPixel(color, x + i, y + j);
        }
    }
}

int64_t setFontScale(uint64_t scale) {
    if (scale < MIN_FONT_SCALE || scale > MAX_FONT_SCALE) {
        return -1;
    }
    fontScale = scale;
    clear();
    return 0;
}

void updateTextCursor(CursorMovementType movementType) {
    uint64_t xOffset = 0, yOffset = 0;
    switch (movementType) {
        case CURSOR_DELETING:
        xOffset = (CHAR_WIDTH * fontScale + 1.5 * HORIZONTAL_PADDING);
        break;
        case CURSOR_TYPING:
        xOffset = -(CHAR_WIDTH * fontScale + HORIZONTAL_PADDING / 2);
        break;
        case CURSOR_NEWLINE:
        xOffset = HORIZONTAL_PADDING / 2;
        break;
        case CURSOR_MOVING:
        // Para cuando usemos las flechas?
        break;
        case CURSOR_PREVIOUS:
        drawRectangle(BORDER_PADDING + HORIZONTAL_PADDING / 2, currentY, 1, CHAR_HEIGHT * fontScale, 0x00000000);
        return;
        default:
        break;
    }
    drawRectangle(currentX + HORIZONTAL_PADDING / 2, currentY, 1, CHAR_HEIGHT * fontScale, 0xFFFFFFFF); // place new one
    drawRectangle(currentX + xOffset, currentY + yOffset, 1, CHAR_HEIGHT * fontScale, 0x0000000); // remove previous one
}
