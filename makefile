CC = g++
CFLAGS = -g -c -Wall
LINK = g++ 
LFLAGS = -g
ARCHIVE = Bride_$(REVISION).tar.gz
REVISION = Roni

a.out			:	bridge.o

main.o			:	main.cpp
					$(CC) $(CFLAGS) main.cpp

clean			:
					rm -f a.out *.o

save			:
					tar cvzf $(ARCHIVE) birdge.cpp makefile
