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
double sample_width, it;

void init_write(int n, float f, float a, const char* filename) {
    sample_rate = n;
    sample_width = 1 / sample_rate;
    frequency = f;
    amplitude = a;

    if (filename != NULL)
        pipeout = open_rendering_pipe(filename);
    else
        pipeout = open_playing_pipe();
}

void write_silence(float duration) { write(duration, silence); }

void write_note(float duration) { write(duration, note); }

void write(float duration, int16_t (*f)(int)) {
    double initial=it, final=it+duration;
    printf("%f\n", sample_width);
    int16_t value;
    for (double t=initial; t<final; t+=sample_width) {
        value = f(t);
        fwrite(&value, 2, 1, pipeout); 
    }
    it = fmod(final, TAU);
}

FILE* open_testing_pipe() {
    return popen("cat > output.bin", "w");
}

FILE* open_playing_pipe() {
    char* command = malloc(MAX_COMMAND_BUFFER * sizeof *command);

    sprintf(command, "%s %s %s %s %s %d %s %s %s %d %s %s",
        FFPLAY_BIN,
        "-nodisp",  // no visual
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
