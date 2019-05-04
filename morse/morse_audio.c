#include <stdio.h>
#include "write_audio.h"

void calculate_units();

float unit = 0, f_unit = 0;

void init_audio(int wpm, int fwpm) {
    if (!wpm && !fwpm)
        wpm = fwpm = 15;  // default
    else if (!fwpm) fwpm = wpm;
    else if (!wpm) wpm = fwpm;

    calculate_units(wpm, fwpm);
    init_write();
}

int write_code(int ch) {
    switch (ch) {
        case '.':
            write_note(unit);
            write_silence(unit);
            break;

        case '-':
            write_note(3.0 * unit);
            write_silence(unit);
            break;

        case ' ':
        case '/':
            write_silence(2.0 * f_unit);
            break;

        case '\n':
            write_silence(6.0 * f_unit);
            break;

        default:
            return EOF;
    }
    return ch;
}

void export_audio(char* filename) {
    correct_audio_buffer(unit);
    render_file(filename);
}

void calculate_units(int wpm, int fwpm) {
    unit = 6.0 / (5.0 * wpm);
    f_unit = ((60.0 / fwpm) - (31.0 * unit)) / 19.0;
}
