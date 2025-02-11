C = gcc
CFLAGS = -Wall -g -I./Librerie 
OBJ = Main.o Simulatore.o rngs.o rvms.o utils.o
EXEC = simulatore


$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) -lm  


Main.o: Main.c Simulatore.h Librerie/rngs.h Librerie/rvms.h Librerie/struct.h Librerie/utils.h
	$(CC) $(CFLAGS) -c Main.c

Simulatore.o: Simulatore.c Simulatore.h Librerie/rngs.h Librerie/struct.h Librerie/utils.h Librerie/rvms.h
	$(CC) $(CFLAGS) -c Simulatore.c 

rngs.o: Librerie/rngs.c Librerie/rngs.h
	$(CC) $(CFLAGS) -c Librerie/rngs.c

rvms.o: Librerie/rvms.c Librerie/rvms.h
	$(CC) $(CFLAGS) -c Librerie/rvms.c 

utils.o: Librerie/utils.c Librerie/utils.h Librerie/rngs.h Librerie/struct.h Librerie/rvms.h
	$(CC) $(CFLAGS) -c Librerie/utils.c


clean:
	rm -f $(OBJ) $(EXEC)