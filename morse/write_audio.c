#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "write_audio.h"

#define N 44100     // Sampling Frequency
#define A 16383.0   // Amplitude of sine wave
#define F 1000.0    // Frequency of sine wave
#define TAU 2.0*M_PI
#define correct(d) d + (0.5 - fmod(d, 0.5))

#define MAX_BUFFER 100
#define FFMPEG_BIN "ffmpeg"
#define FILENAME "morse.wav"

void update_buffer_length();
int16_t silence(int);
int16_t note(int);
void write(float, int16_t (*)(int));

int16_t *audio_buffer, *head;
int buffer_length;
float buffer_duration;

void init_write() {
    head = audio_buffer = NULL;
    buffer_length = 0;
    buffer_duration = 0;
}

void write_silence(float duration) { write(duration, silence); }

void write_note(float duration) { write(duration, note); }

void write(float duration, int16_t (*f)(int)) {
    int samples = duration * N;
    buffer_duration += duration;
    buffer_length += samples;
    update_buffer_length(samples);
    for (int t = 0; t < samples; t++) *(audio_buffer++) = f(t);
}

void correct_audio_buffer(float unit) {
    buffer_duration -= unit;  // remove extra unit of silence

    // add padding to round off to a multiple of 0.5 seconds
    write_silence(correct(buffer_duration) - buffer_duration);
}

void render_file() {
    // Pipe the audio data to ffmpeg, which writes it to a wav file
    FILE* pipeout;

    char* command = malloc(100 * sizeof(char));
    sprintf(command, "%s %s %s %s %s %d %s %s %s %d %s %s %s",
        FFMPEG_BIN,
        "-y",  // overwrite output file if it exists
        "-f", "s16le",
        "-ar", N,  // audio sampling frequency
        "-i", "-",  // The input comes from a pipe
        "-ac", 1,  // Tells FFMPEG to expect an audio channel
        "-loglevel", "error",
        FILENAME
    );

    pipeout = popen(command, "w");
    fwrite(head, 2 * buffer_duration, N, pipeout);
    pclose(pipeout);
}

void update_buffer_length(int added_length) {
    head = realloc(head, (buffer_length + 1) * sizeof(int16_t));
    audio_buffer = head + (buffer_length - added_length);
}

int16_t silence(int t) { return 0; }

int16_t note(int t) {
    return A*sin((TAU * F * t) / (float)N);
}


int main() {
    float wpm = 15;
    float unit = 6.0 / (5.0 * wpm);
    init_write();

    write_note(unit);
    write_silence(unit);
    write_note(3.0 * unit);
    write_silence(unit);
    write_note(3.0 * unit);
    write_silence(unit);
    write_note(unit);
    write_silence(unit);

    write_silence(2.0 * unit);

    write_note(unit);
    write_silence(unit);
    write_note(3.0 * unit);
    write_silence(unit);

    write_silence(2.0 * unit);

    write_note(unit);
    write_silence(unit);
    write_note(3.0 * unit);
    write_silence(unit);
    write_note(unit);
    write_silence(unit);

    write_silence(2.0 * unit);
    write_silence(unit);

    write_note(unit);
    write_silence(unit);
    write_note(unit);
    write_silence(unit);

    write_silence(2.0 * unit);

    write_note(unit);
    write_silence(unit);
    write_note(unit);
    write_silence(unit);
    write_note(unit);
    write_silence(unit);

    correct_audio_buffer(unit);

    render_file();
    return 0;
}
