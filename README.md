# cryptogram
A toolset for practicing cryptography

## Description
I have always been interested in cryptographic puzzles. When I was beginning to
write my first lines of code with my father as my teacher, I discovered in the
codebase of one of his projects, a very simple caesar cipher that he used to
encrypt dates.

Since then I've come come across the term __cryptography__ in so many places like
for example Movies ([The Prestige](https://www.imdb.com/title/tt0482571/)), and
even I put together a basic caesar cipher algorithm to encrypt data files which
were actually exposed to the filesystem and were read in by a C++ program I wrote
to store names of my friends.

I became hungrier for puzzles as I learned about more and more cryptographic
algorithms. This project however doesn't provide me with puzzles, but it is
definitely helpful while solving them. I mainly find the puzzles at
[r/codes](https://old.reddit.com/r/codes/) quite interesting sometimes.

The main idea behind this project was also to break multi layer ciphers, like
for example a message encrypted with algorithm A and then algorithm B can be
decrypted by just piping the decryption of algorithm B as the input of
decryption algorithm A in order to get the original message. More on pipes
below.

## Installation
1. Clone
   ```sh
   git clone https://github.com/aritra1911/cryptogram.git
   ```

2. cd
   ```sh
   cd cryptogram
   ```

3. Make sure you got `gcc` installed.

4. Make
   ```sh
   make all
   ```
   or
   ```sh
   make
   ```
   To make everything all at once, or put the name of a specific tool like
   ```sh
   make *tool_name*
   ```
   For example:
   ```sh
   make vigenere
   ```
   Will build and compile the vigenere cipher tool for you, which is a really
   good cipher.

5. Play
   ```sh
   echo "Happy Birthday" | ./vigenere.out -k liza
   ```

## Structure of each tool
Each of them primarily reads input from `stdin` and outputs to `stdout`, however
a specific file could be passed as an optional argument to all of them. The
morse tool is a bit different in the sense that it will output the audio directly
to a file. I might add an option later to write to `stdout` so it can be piped to
another application to play the code instantly. Is it possible to determine if
there is a pipe available at the end? I don't want users to complain about their
terminals getting dirty.

### Pipes
Say for example you got a code that says:
```
..-. ...- -... -.-. .--/.-- ..- . .-. -.-. .--. -. .--
```
You know it's morse code so you put it through the morse tool:
```sh
echo "..-. ...- -... -.-. .--/.-- ..- . .-. -.-. .--. -. .--" | ./morse.out -d
```
You get:
```
fvbcw wuercpnw
```
You have a name to serve as the key, you try the vigenere cipher:
```sh
echo "fvbcw wuercpnw" | ./vigenere.out -dk liza
```
You get:
```
unccl oveguqnl
```
You are confused that it's not really making any sense and doubting if you're
missing a decrytion algorithm in between, but you apply a ROT-13 (a.k.a. caesar
cipher with offset 13) anyway.
```sh
echo "unccl oveguqnl" | ./caesar.out -do 13
```
And Lo' behold! You got:
```
happy birthday
```
Hopefully none of your friends are this evil that they will encrypt a wish with
three layers of encryption.

But all that was not necessary. Yes you figured out the chain, but now you can
take the help of `pipes` to demonstrate your code breaking abilities in just one
line:
```sh
echo "..-. ...- -... -.-. .--/.-- ..- . .-. -.-. .--. -. .--" | \
    ./morse.out -d | \
    ./vigenere.out -dk liza | \
    ./caesar.out -do 13
```
And you get:
```
happy birthday
```

Pipes are incredible and is something I've taken into deep consideration while
building this project, such that all of the tools are capable of inputting or
outputting from or to a pipe, respectively. Also this leaves me from programming
a lot of jargon that comes with building a proper UI. Have a look at this
[video](https://youtu.be/tc4ROCJYbm0?t=249) where Brian Kernighan shows you how
Unix pipes work. He was a really cool guy.

## FAQs
### Where are the help pages?
On their way! I'm pretty lazy for these kind of things, but I do understand
their necessity. The flags are different for different tools and sometimes even
I forget how to use them. For now, you can just open the source files and hack
your way to reach the flag definitions.

### What's wrong with the Morse tool?
I know it's quite buggy and I also have quite poor about explaining my code. It
looks really dreadful, I agree. But it's also the tool that I spent most time
programming than any other tool in this repo. Even I had to fight my way through
in understanding what was exactly happening when I was recently making a change
in its core. Eh! I'm sorry, I'm a not a very good programmer. Do you know where I
can learn to properly document my code?

### Can you explain how the Morse tool works?
Sure! As you might've expected, it's using a binary tree which is serialized in
`morse/tree.dat`. My program simply deserializes the file and make the tree
before proceeding to take input. Once the tree is ready, it's quite easy to find
the morse code for a character, you just keep pushing a `.` whenever you take a
left branch or a `-` whenever you take a right. The function for this is
recursive and thus makes the code much more efficient. This function is defined
in the main `morse.c` file and is called `put_morse`. Similarly decryption takes
place by finding the character by following a code string, i.e. taking the left
branch if the charcter is a `.` and taking a right when the character is a `-`.
There isn't just another function for this. The `demorse` function defined in
the main file `morse.c` handles this quite naturally.

For the audio, I did not really wanna code a writer. So I just piped binary data
into `ffmpeg` and turned it into a `.wav` file. The thing that was previously
buggy about this is that the Farnsworth speed could not be exactly followed due
to the spaces surrounding `/` during an interword spacing. So I realized that
the spaces weren't serving any real purpose and I removed them and adapted the
program accordingly. Now everything should be perfect.

I actually wrote a program for serializing into the file `tree.dat`, but I
considered it as an example of bad code and did not wish to commit. However, you
can find it
[here](https://gist.github.com/aritra1911/4277a78294878329354eb51790ee75f8).

### How is your ESP32 Morse Code Transmitter working?
> *Ref*: c24be7aa9596b91dc28b263047cb8f93a580a641

It worked pretty good until I burnt its processor while soldering a cap on the
board to automatically trigger flash mode. I bought another one. It's output was
current limited for a buzzer so I had to set up a transistor (good ol' 2n3904)
circuit in order to get it up and running.
