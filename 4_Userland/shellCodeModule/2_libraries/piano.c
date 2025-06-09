// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <piano.h>

//DO:W, RE:E, MI:R, FA:T, SOL:Y, LA:U, SI:I
//DO#:3, RE#:4, FA#:6, SOL#:7, LA#:8

void playKeys() {
    char pressedKey;
    double octaveShifter = 1; //Octave C4
    while((pressedKey = getchar()) != 'q') {
        if(isUpper(pressedKey)){
            octaveShifter = 2;
        }
        if(!isNumber(pressedKey)) {
            pressedKey = toLowercase(pressedKey);
        }
        switch (pressedKey) {
        case 'w':   //DO
            beep(C * octaveShifter, 100);
            break;
        case '3':   //DO#
            beep(C_SHARP * octaveShifter, 200);
            break;
        case 'e':   //RE
            beep(D * octaveShifter, 200);
            break;
        case '4':   //RE#
            beep(D_SHARP * octaveShifter, 200);
            break;
        case 'r':   //MI
            beep(E * octaveShifter, 200);
            break;
        case 't':   //FA
            beep(F * octaveShifter, 200);
            break;
        case '6':   //FA
            beep(F_SHARP * octaveShifter, 200);
            break;
        case 'y':   //SOL
            beep(G * octaveShifter, 200);
            break;
        case '7':   //SOL
            beep(G_SHARP * octaveShifter, 200);
            break;
        case 'u':   //LA
            beep(A * octaveShifter, 200);
            break;
        case '8':   //LA
            beep(A_SHARP * octaveShifter, 200);
            break;
        case 'i':   //SI
            beep(B * octaveShifter, 200);
            break;
        case 'o':   //SI
            beep(C*2 * octaveShifter, 200);
            break;
        default:
            break;
        }
    }
}
