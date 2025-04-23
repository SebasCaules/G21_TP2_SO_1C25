#include <stdint.h>
#include <syscallsInt.h>
#include <stdLib.h>
#include <strLib.h>

#define NUMBER_OF_SONGS 5

#define D_SHARP_4 311    // D#4
#define F4 349           // F4
#define F_SHARP_4 370    // F#4
#define G4 392           // G4
#define G_SHARP_4 415    // G#4
#define A_SHARP_4 466    // A#4
#define C5 523           // C5
#define D5 587           // D5

#define D2 73    // D2
#define G2 98    // G2 
#define A2 110   // A2 
#define D3 146   // D3 
#define F3 175   // F3

#define C_SHARP_4 277  // C#4 frequency in Hz
#define D4 293        // D4 frequency in Hz
#define B4 494        // B4 frequency in Hz
#define A4 440  // A4 frequency in Hz

typedef enum {
    REDONDA = 16,
    BLANCA = 8,
    NEGRA = 4,
    CORCHEA = 2,
    SEMI_CORCHEA = 1
} NoteTypes;

typedef struct {
    uint64_t freq;
    NoteTypes type;
    char isSilence;
} Note;

typedef struct {
    Note* notes;
    uint64_t bpm;
    uint64_t numberOfNotes;
} Song;


/**
 * @brief Plays a song from it's db byt hte number passed by the user
*/
void playSong(char number);

