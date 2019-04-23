all: caesar vigenere

caesar: caesar.o shifts.o
	gcc caesar.o shifts.o -o caesar.out

vigenere: vigenere.o shifts.o
	gcc vigenere.o shifts.o -o vigenere.out

caesar.o: caesar/caesar.c
	gcc -c caesar/caesar.c

vigenere.o: vigenere/vigenere.c
	gcc -c vigenere/vigenere.c

shifts.o: shifts/shifts.c
	gcc -c shifts/shifts.c

clean:
	rm *.o *.out
