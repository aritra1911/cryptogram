all: caesar morse vigenere

caesar: caesar.o shifts.o
	gcc caesar.o shifts.o -o caesar.out

morse: morse.o tree_utils.o morse_audio.o write_audio.o
	gcc morse.o tree_utils.o morse_audio.o write_audio.o -o morse.out -lm

vigenere: vigenere.o shifts.o
	gcc vigenere.o shifts.o -o vigenere.out

caesar.o: caesar/caesar.c
	gcc -c caesar/caesar.c

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
