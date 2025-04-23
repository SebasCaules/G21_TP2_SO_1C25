#include <stdint.h>
#include <stdLib.h>
#include <strLib.h>

typedef enum {
    C = 261,
    C_SHARP = 277,
    D = 293,
    D_SHARP = 311,
    E = 329,
    F = 349,
    F_SHARP = 370,
    G = 392,
    G_SHARP = 415,
    A = 440,
    A_SHARP = 466,
    B = 494
} middleCNotes;


/**
 * @brief Get the keys pressed and plays the frequencies according to the mapping of each key
 * 1 - The scale of C4
 * 2 - The happy birtday song
 * 3 - Adios nonino, Astor Piazzola
*/
void playKeys();