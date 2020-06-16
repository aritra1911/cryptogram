#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "write_audio.h"

#define TAU 2.0*M_PI
#define MAX_COMMAND_BUFFER 100
#define FFPLAY_BIN "ffplay"
#define FFMPEG_BIN "ffmpeg"

#define correct(d) d + (0.5 - fmod(d, 0.5))

int16_t silence(int);
int16_t note(int);
void write(float, int16_t (*)(int));
FILE* open_testing_pipe();
FILE* open_playing_pipe();
FILE* open_rendering_pipe(const char*);

char* filename;
FILE* pipeout;
int sample_rate;
float frequency, amplitude;

void init_write(int n, float f, float a, const char* filename) {
    sample_rate = n;
    frequency = f;
    amplitude = a;
    //pipeout = open_testing_pipe();
    //return;

    if (filename != NULL)
        pipeout = open_rendering_pipe(filename);
    else
        pipeout = open_playing_pipe();
}

void write_silence(float duration) { write(duration, silence); }

void write_note(float duration) { write(duration, note); }

void write(float duration, int16_t (*f)(int)) {
    int16_t value;
    int samples = duration * sample_rate;
    for (int t=0; t<samples; t++) {
        value = f(t);
        fwrite(&value, 2, 1, pipeout); 
    }
}

FILE* open_testing_pipe() {
    return popen("cat > output.bin", "w");
}

FILE* open_playing_pipe() {
    char* command = malloc(MAX_COMMAND_BUFFER * sizeof *command);

    sprintf(command, "%s %s %s %s %s %d %s %s %s %d %s %s",
        FFPLAY_BIN,
        //"-nodisp",  // no visual
        "-autoexit",
        "-f", "s16le",
        "-ar", sample_rate,  // audio sampling frequency
        "-i", "-",  // The input comes from a pipe
        "-ac", 1,  // Tells FFPLAY to expect an audio channel
        "-loglevel", "error"
    );

    return popen(command, "w");
}

FILE* open_rendering_pipe(const char* filename) {
    char* command = malloc(MAX_COMMAND_BUFFER * sizeof *command);

    sprintf(command, "%s %s %s %s %s %d %s %s %s %d %s %s %s",
        FFMPEG_BIN,
        "-y",  // overwrite output file if it exists
        "-f", "s16le",
        "-ar", sample_rate,  // audio sampling frequency
        "-i", "-",  // The input comes from a pipe
        "-ac", 1,  // Tells FFMPEG to expect an audio channel
        "-loglevel", "error",
        filename
    );

    return popen(command, "w");
}

void close_audio_pipe() {
    pclose(pipeout);
}

int16_t silence(int t) { return 0; }

int16_t note(int t) {
    return amplitude * sin((TAU * frequency * t) / (float)sample_rate);
}
