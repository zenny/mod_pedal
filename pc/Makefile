CC=gcc
CFLAGS=--std=c99 -Wall
LIBS=-lX11 -lXtst

OBJ=main.o x11.o serial.o
EXEC=pedald

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	touch _dummy.o
	rm *.o
	if [ -f ./$(EXEC) ]; then rm ./$(EXEC); fi
