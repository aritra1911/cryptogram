CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

all: atbash base64 caesar morse sbx_breaker vigenere xor

atbash: atbash.o shifts.o
	$(CC) atbash.o shifts.o -o atbash.out

base64: base64.o
	$(CC) base64.o -o base64.out

caesar: caesar.o shifts.o
	$(CC) caesar.o shifts.o -o caesar.out

hex: hex.o
	$(CC) hex.o -o hex.out

morse: morse.o tree_utils.o morse_audio.o write_audio.o
	$(CC) morse.o tree_utils.o morse_audio.o write_audio.o -o morse.out -lm

sbx_breaker: sbx_breaker.o
	$(CC) sbx_breaker.o -o sbx_breaker.out -lm

vigenere: vigenere.o shifts.o
	$(CC) vigenere.o shifts.o -o vigenere.out

xor: xor.o
	$(CC) xor.o -o xor.out

atbash.o: atbash/atbash.c
	$(CC) $(CFLAGS) -c atbash/atbash.c

base64.o: base64/base64.c
	$(CC) $(CFLAGS) -c base64/base64.c

caesar.o: caesar/caesar.c
	$(CC) $(CFLAGS) -c caesar/caesar.c

hex.o: hex/hex.c
	$(CC) $(CFLAGS) -c hex/hex.c

morse.o: morse/morse.c
	$(CC) $(CFLAGS) -c morse/morse.c

morse_audio.o: morse/morse_audio.c
	$(CC) $(CFLAGS) -c morse/morse_audio.c

sbx_breaker.o: sbx_breaker/sbx_breaker.c
	$(CC) $(CFLAGS) -c sbx_breaker/sbx_breaker.c

shifts.o: shifts/shifts.c
	$(CC) $(CFLAGS) -c shifts/shifts.c

tree_utils.o: morse/tree_utils.c
	$(CC) $(CFLAGS) -c morse/tree_utils.c

vigenere.o: vigenere/vigenere.c
	$(CC) $(CFLAGS) -c vigenere/vigenere.c

write_audio.o: morse/write_audio.c
	$(CC) $(CFLAGS) -c morse/write_audio.c

xor.o: xor/xor.c
	$(CC) $(CFLAGS) -c xor/xor.c

clean:
	rm *.o *.out
