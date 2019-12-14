all: base64 caesar morse vigenere

base64: base64.o
	gcc base64.o -o base64.out

caesar: caesar.o shifts.o
	gcc caesar.o shifts.o -o caesar.out

hex: hex.o
	gcc hex.o -o hex.out

morse: morse.o tree_utils.o morse_audio.o write_audio.o
	gcc morse.o tree_utils.o morse_audio.o write_audio.o -o morse.out -lm

vigenere: vigenere.o shifts.o
	gcc vigenere.o shifts.o -o vigenere.out

base64.o: base64/base64.c
	gcc -c base64/base64.c

caesar.o: caesar/caesar.c
	gcc -c caesar/caesar.c

hex.o:
	gcc -c hex/hex.c

morse.o: morse/morse.c
	gcc -c morse/morse.c

morse_audio.o: morse/morse_audio.c
	gcc -c morse/morse_audio.c

shifts.o: shifts/shifts.c
	gcc -c shifts/shifts.c

tree_utils.o: morse/tree_utils.c
	gcc -c morse/tree_utils.c

vigenere.o: vigenere/vigenere.c
	gcc -c vigenere/vigenere.c

write_audio.o: morse/write_audio.c
	gcc -c morse/write_audio.c

clean:
	rm *.o *.out
