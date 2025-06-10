// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <spotify.h>

static Note do4_scale[] = {
    {261, NEGRA, 0},  // C4
    {293, NEGRA, 0},  // D4
    {329, NEGRA, 0},  // E4
    {349, NEGRA, 0},  // F4
    {392, NEGRA, 0},  // G4
    {440, NEGRA, 0},  // A4
    {493, NEGRA, 0},  // B4
    {523, NEGRA, 0}   // C5
};

static Note happy_birthday_notes[] = {
    // First line
    {392, NEGRA, 0}, {392, CORCHEA, 0}, {440, NEGRA, 0}, {392, NEGRA, 0},
    {523, NEGRA, 0}, {494, BLANCA, 0},
    // Second line
    {392, NEGRA, 0}, {392, CORCHEA, 0}, {440, NEGRA, 0}, {392, NEGRA, 0},
    {587, NEGRA, 0}, {523, BLANCA, 0},
    // Third line
    {392, NEGRA, 0}, {392, CORCHEA, 0}, {784, NEGRA, 0}, {659, NEGRA, 0},
    {523, NEGRA, 0}, {494, NEGRA, 0}, {440, BLANCA, 0},
    // Fourth line
    {698, NEGRA, 0}, {698, CORCHEA, 0}, {659, NEGRA, 0}, {523, NEGRA, 0},
    {587, NEGRA, 0}, {523, BLANCA, 0}
};


static Note adios_nonino_notes[] = {
	{D_SHARP_4, BLANCA, 0}, {D_SHARP_4, NEGRA, 0}, {A_SHARP_4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0},
	{D_SHARP_4, BLANCA, 0}, {D_SHARP_4, NEGRA, 0}, {C5, CORCHEA, 0}, {A_SHARP_4, CORCHEA, 0},
	{D_SHARP_4, BLANCA, 0}, {D_SHARP_4, NEGRA, 0}, {F4, CORCHEA, 0}, {F_SHARP_4, CORCHEA, 0},
	{F_SHARP_4, BLANCA, 0}, {F4, NEGRA, 0}, {F4, NEGRA, 0},
	{F4, BLANCA, 0}, {F4, NEGRA, 0}, {C5, CORCHEA, 0}, {A_SHARP_4, CORCHEA, 0},
	{F4, BLANCA, 0}, {F4, NEGRA, 0}, {D5, SEMI_CORCHEA, 0}, {C5, CORCHEA, 0}, {A_SHARP_4, SEMI_CORCHEA, 0},
	{F4, BLANCA, 0}, {F4, NEGRA, 0}, {G4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0},
	{G_SHARP_4, BLANCA, 0}, {G4, NEGRA, 0}, {G4, NEGRA, 0}
};

static Note come_together_notes[] = {
	{D2, CORCHEA, 0}, {D2, CORCHEA, 0}, {G2, SEMI_CORCHEA, 0}, {A2, SEMI_CORCHEA, 0}, {F3, CORCHEA}, {F3, NEGRA, 0}, {D3, NEGRA, 0},
	{D2, CORCHEA, 0}, {D2, CORCHEA, 0}, {G2, SEMI_CORCHEA, 0}, {A2, SEMI_CORCHEA, 0}, {F3, CORCHEA}, {F3, NEGRA, 0}, {D3, NEGRA, 0},
	{D2, CORCHEA, 0}, {D2, CORCHEA, 0}, {G2, SEMI_CORCHEA, 0}, {A2, SEMI_CORCHEA, 0}, {F3, CORCHEA}, {F3, NEGRA, 0}, {D3, NEGRA, 0},
	{D2, CORCHEA, 0}, {D2, CORCHEA, 0}, {G2, SEMI_CORCHEA, 0}, {A2, SEMI_CORCHEA, 0}, {F3, CORCHEA}, {F3, NEGRA, 0}, {D3, NEGRA, 0}
};

static Note lalaland_notes[] = {
	{C_SHARP_4, BLANCA, 0}, {F_SHARP_4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0}, {A4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0}, {F_SHARP_4, CORCHEA, 0}, {D4, CORCHEA + NEGRA, 0},
	{C5, SEMI_CORCHEA/4, 1}, {D4, SEMI_CORCHEA * 3, 0}, {F_SHARP_4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0}, {A4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0}, {F_SHARP_4, CORCHEA, 0}, {C_SHARP_4, CORCHEA + NEGRA, 0},
	{C5, SEMI_CORCHEA/4, 1}, {C_SHARP_4, SEMI_CORCHEA * 3, 0}, {F_SHARP_4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0}, {A4, CORCHEA, 0}, {G_SHARP_4, CORCHEA, 0}, {F_SHARP_4, CORCHEA, 0}, {D4, CORCHEA + NEGRA, 0},
	{B4, NEGRA}, {A4, CORCHEA}, {G_SHARP_4, CORCHEA}, {A4, CORCHEA}, {G_SHARP_4, CORCHEA}, {F_SHARP_4, CORCHEA}, {C_SHARP_4, NEGRA}
};

static Song songList[NUMBER_OF_SONGS] = {
	{do4_scale, 240, sizeof(do4_scale) / sizeof(Note)}, 
	{happy_birthday_notes, 120, sizeof(happy_birthday_notes) / sizeof(Note)}, 
	{adios_nonino_notes, 78, sizeof(adios_nonino_notes) / sizeof(Note)},
	{come_together_notes, 82, sizeof(come_together_notes) / sizeof(Note)},
	{lalaland_notes, 112, sizeof(lalaland_notes) / sizeof(Note)}
};

void playSong(char number) {
	number -= 48; //Pasa el ascii a int
	Song song = songList[number - 1]; // Le resta 1 para acceder a la cancion correcta
	double timePerQuarter = (60000.0 / song.bpm);
	for (int i = 0; i < song.numberOfNotes; i++) {
		Note notei = song.notes[i];
		double duration = (notei.type / 4.0) * timePerQuarter; 
		if(!notei.isSilence) {
			beep(notei.freq, duration);
		} else {
			sys_sleep(duration);
		}
	}
}
