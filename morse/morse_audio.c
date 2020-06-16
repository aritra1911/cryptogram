// Acknowledgement : https://morsecode.world/international/timing.html

#include <stdio.h>
#include <stdbool.h>
#include "write_audio.h"

float unit, f_unit;
extern int previous_character;

void init_audio(int wpm, int fwpm, int n, float f, float a, const char* fn) {
    // calculate units
    unit = 6.0 / (5.0 * wpm);
    f_unit = ((60.0 / fwpm) - (31.0 * unit)) / 19.0;

    init_write(n, f, a, fn);
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

        case ' ':  // Inter-character spacing
            if (previous_character == '.' || previous_character == '-') {
                /* Here a difference is added as the last dit or dah already
                   wrote a unit of silence. Adding this difference makes that
                   last unit of silence an f_unit of silence and hence getting
                   3 total f_units of silence.
                */
                write_silence(2.0 * f_unit + (f_unit - unit));
                break;
            }
            // The previous_character was a space having no hanging units
            write_silence(3.0 * f_unit);
            break;

        case  '/':
        case '\n':  // Inter-word spacing
            // Same thing as previous, add the diff to make the last unit an
            // f_unit.
            if (previous_character == '.' || previous_character == '-')
                write_silence(6.0 * f_unit + (f_unit - unit));
            else
                // The previous_character was a space having no hanging units
                write_silence(7.0 * f_unit);
            break;

        default:
            return EOF;
    }
    previous_character = ch;
    return ch;
}

void finish_writing() {
    close_audio_pipe();
}
