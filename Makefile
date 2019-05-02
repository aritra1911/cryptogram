all: caesar morse vigenere

caesar: caesar.o shifts.o
	gcc caesar.o shifts.o -o caesar.out

morse: morse.o tree_utils.o
	gcc morse.o tree_utils.o -o morse.out

vigenere: vigenere.o shifts.o
	gcc vigenere.o shifts.o -o vigenere.out

caesar.o: caesar/caesar.c
	gcc -c caesar/caesar.c

morse.o: morse/morse.c
	gcc -c morse/morse.c

shifts.o: shifts/shifts.c
	gcc -c shifts/shifts.c

tree_utils.o: morse/tree_utils.c
	gcc -c morse/tree_utils.c

vigenere.o: vigenere/vigenere.c
	gcc -c vigenere/vigenere.c

clean:
	rm *.o *.out
