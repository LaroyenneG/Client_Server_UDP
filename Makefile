OBJ1 := client.o
EXENAME1 := client

OBJ2 := serveur.o
EXENAME2 := serveur

CC := gcc
CFLAGS := -Wall -O3 -g

all : exe

exe : $(EXENAME1)

$(EXENAME1) : $(OBJ1)
	gcc -g $^ -o $@


exe : $(EXENAME2)

$(EXENAME2) : $(OBJ2)
	gcc -g $^ -o $@



clean:
	rm -f $(OBJ1)
	rm -f $(OBJ2)
	rm -f *~
	rm -f $(EXENAME1)
	rm -f $(EXENAME2)
