vigenere: vigenere.o shifts.o
	gcc vigenere.o shifts.o -o vigenere

vignere.o: vigenere.c
	gcc -c vigenere.c

shifts.o: ../shifts/shifts.c
	gcc -c ../shifts/shifts.c

clean:
	rm *o vigenere
