CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99 -g
OBJECTS = banhammer.o hash.o bf.o aes.o ll.o lex.yy.o

.PHONY: all
all: banhammer

banhammer: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(CFLAGS) -o banhammer -lfl

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c -g

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c -g

lex.yy.o : lex.yy.c
	cc -c lex.yy.c

lex.yy.c: words.l
	flex words.l

aes.o: aes.c
	$(CC) $(CFLAGS) -c aes.c -g

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c -g

ll.o: ll.c
	$(CC) $(CFLAGS) -c ll.c -g

.PHONY: clean
clean:
	rm -f $(OBJECTS) lex.yy.c banhammer
